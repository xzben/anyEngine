#include "LogicDevice.h"

#include <unordered_map>

#include "../VulkanCommandPool.h"
#include "../VulkanQueue.h"
#include "Instance.h"
#include "PhysicalDevice.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

LogicDevice::LogicDevice(const Instance& instance,
                         const PhysicalDevice& physical_device, VkDevice device,
                         EmbeddingQueues& embedding_queues,
                         std::vector<QueueInfo> custom_queues)
    : m_device(device),
      m_instance(instance),
      m_physicalDevice(physical_device) {
    for (auto i = 0; i < QueueType::Count; ++i) {
        auto& info = embedding_queues.queue[i];
        for (auto j = 0; j < info.count; ++j) {
            auto queue = new VulkanQueue(*this, info.familyIndex, j);
            m_embeddingQueues[i].emplace_back(queue);
        }
    }

    for (auto& info : custom_queues) {
        std::vector<Queue*> queues;
        queues.reserve(info.count);
        for (auto i = 0; i < info.count; ++i) {
            auto queue = new VulkanQueue(*this, info.familyIndex, i);
            queues.emplace_back(queue);
        }
        m_customQueues.emplace(info.familyIndex, std::move(queues));
    }

    // TODO: use reset pool for performance
    m_commandPool =
        createCommandPool(QueueType::Graphics, ResetMode::AlwaysAllocate);

    // vma
    VmaAllocatorCreateInfo vma_info = {};
    vma_info.physicalDevice         = physical_device;
    vma_info.device                 = m_device;
    vma_info.instance               = instance;
#if VMA_DYNAMIC_VULKAN_FUNCTIONS
    static VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr     = vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr       = vkGetDeviceProcAddr;
    vma_info.pVulkanFunctions                 = &vulkanFunctions;
#endif
    auto result = vmaCreateAllocator(&vma_info, &m_allocator);
    assert(result == VK_SUCCESS);
}

LogicDevice::~LogicDevice() {
    if (m_commandPool) {
        delete m_commandPool;
        m_commandPool = nullptr;
    }

    if (m_allocator) {
        vmaDestroyAllocator(m_allocator);
    }
    if (m_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_device, nullptr);
    }
}

void LogicDevice::waitIdle() const { vkDeviceWaitIdle(m_device); }

std::optional<uint32_t> LogicDevice::getQueueFamilyIndex(QueueType t) const {
    if (m_embeddingQueues[t].empty()) {
        return std::nullopt;
    }
    return m_embeddingQueues[t].front()->familyIndex();
}

VulkanQueue* LogicDevice::getQueue(QueueType t, int index) const {
    assert(m_embeddingQueues[t].size() > index);
    return m_embeddingQueues[t][index];
}

VulkanCommandPool* LogicDevice::createCommandPool(QueueType queue_type,
                                                  ResetMode mode) const {
    auto index = getQueueFamilyIndex(queue_type);
    if (index) {
        return new VulkanCommandPool(*this, index.value(), mode);
    }

    return nullptr;
}

VulkanCommandBuffer* LogicDevice::requestCommandBuffer() const {
    return m_commandPool->alloc(CommandBufferLevel::PRIMARY);
}

std::unique_ptr<LogicDevice> LogicDeviceBuilder::build() {
    auto extensions = m_extensions;

    if (m_requirePresent) {
        extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    auto embeddingQueues = buildEmbeddingQueues();
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    queueInfos.resize(m_customQueues.size() + embeddingQueues.size(),
                      {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO});

    auto i = 0;
    for (; i < m_customQueues.size(); i++) {
        queueInfos[i].queueFamilyIndex = m_customQueues[i].familyIndex;
        queueInfos[i].queueCount       = m_customQueues[i].priorities.size();
        queueInfos[i].pQueuePriorities = m_customQueues[i].priorities.data();
    }

    for (auto j = 0; j < embeddingQueues.size(); i++, j++) {
        queueInfos[i].queueFamilyIndex = embeddingQueues[j].familyIndex;
        queueInfos[i].queueCount       = embeddingQueues[j].priorities.size();
        queueInfos[i].pQueuePriorities = embeddingQueues[j].priorities.data();
    }

    // device
    VkDeviceCreateInfo deviceInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    deviceInfo.queueCreateInfoCount    = queueInfos.size();
    deviceInfo.pQueueCreateInfos       = queueInfos.data();
    deviceInfo.enabledExtensionCount   = extensions.size();
    deviceInfo.ppEnabledExtensionNames = extensions.data();
    deviceInfo.pEnabledFeatures        = &m_features;
    deviceInfo.enabledLayerCount       = m_validationLayers.size();
    deviceInfo.ppEnabledLayerNames     = m_validationLayers.data();

    VkDevice device;
    if (vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &device)
        != VK_SUCCESS) {
        return nullptr;
    }

    std::vector<QueueInfo> customQueues(m_customQueues.size());
    for (auto i = 0; i < m_customQueues.size(); ++i) {
        customQueues[i].familyIndex = m_customQueues[i].familyIndex;
        customQueues[i].count       = m_customQueues[i].priorities.size();
    }

    return std::make_unique<LogicDevice>(m_instance, m_physicalDevice, device,
                                         m_embeddingQueues,
                                         std::move(customQueues));
}

LogicDeviceBuilder& LogicDeviceBuilder::requireExtension(
    const char* extension) {
    m_extensions.push_back(extension);
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireFeatures(
    const VkPhysicalDeviceFeatures& features) {
    m_features = features;
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireValidationLayer(
    const char* valid_layer) {
    m_validationLayers.push_back(valid_layer);
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::requirePresentQueue(
    uint32_t count, bool perferSperate) {
    auto index = m_physicalDevice.getPresentQueueFamily(m_surface);
    assert(index);

    m_embeddingQueues.queue[QueueType::Present].count       = count;
    m_embeddingQueues.queue[QueueType::Present].familyIndex = index.value();
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireGraphicsQueue(
    uint32_t count, bool perferSperate) {
    auto index = m_physicalDevice.getGraphicsQueueFamily(perferSperate);
    assert(index);
    m_embeddingQueues.queue[QueueType::Graphics].count       = count;
    m_embeddingQueues.queue[QueueType::Graphics].familyIndex = index.value();
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireComputeQueue(
    uint32_t count, bool perferSperate) {
    auto index = m_physicalDevice.getComputeQueueFamily(perferSperate);
    assert(index);
    m_embeddingQueues.queue[QueueType::Compute].count       = count;
    m_embeddingQueues.queue[QueueType::Compute].familyIndex = index.value();
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireTransferQueue(
    uint32_t count, bool perferSperate) {
    auto index = m_physicalDevice.getTransferQueueFamily(perferSperate);
    assert(index);
    m_embeddingQueues.queue[QueueType::Transfer].count       = count;
    m_embeddingQueues.queue[QueueType::Transfer].familyIndex = index.value();
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::requireQueue(
    uint32_t queue_index, std::vector<float> priorities) {
    m_customQueues.emplace_back(queue_index, priorities);
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireQueue(uint32_t queue_index,
                                                     float priorities) {
    m_customQueues.emplace_back(queue_index, priorities);
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::requirePresent() {
    m_requirePresent = true;
    return *this;
}

std::vector<DeviceQueue> LogicDeviceBuilder::buildEmbeddingQueues() {
    std::unordered_map<uint32_t, uint32_t> indexCount;

    for (auto i = 0; i < QueueType::Count; i++) {
        auto& queue = m_embeddingQueues.queue[i];
        if (queue.count <= 0) continue;

        auto it = indexCount.find(queue.familyIndex);
        if (it == indexCount.end()) {
            indexCount.insert(std::make_pair(queue.familyIndex, queue.count));
        } else {
            it->second += queue.count;
        }
    }

    std::vector<DeviceQueue> deviceQueues;
    for (auto it = indexCount.begin(); it != indexCount.end(); it++) {
        deviceQueues.emplace_back(it->first,
                                  std::vector<float>(it->second, 1.0f));
    }

    return deviceQueues;
}

END_VK_NAMESPACE
END_GFX_NAMESPACE