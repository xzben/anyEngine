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
                         EmbeddingQueues& embedding_queues)
    : m_device(device),
      m_instance(instance),
      m_physicalDevice(physical_device) {
    for (auto i = 0; i < QueueType::Count; ++i) {
        auto& info = embedding_queues.queue[i];
        for (auto j = 0; j < info.count; ++j) {
            auto queue = new VulkanQueue(*this, (QueueType)i, info.prioritys[j],
                                         info.familyIndex, j);
            m_embeddingQueues[i].emplace_back(queue);
        }
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
    queueInfos.resize(embeddingQueues.size(),
                      {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO});

    for (int i = 0; i < embeddingQueues.size(); i++) {
        queueInfos[i].queueFamilyIndex = embeddingQueues[i].familyIndex;
        queueInfos[i].queueCount       = embeddingQueues[i].priorities.size();
        queueInfos[i].pQueuePriorities = embeddingQueues[i].priorities.data();
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

    return std::make_unique<LogicDevice>(m_instance, m_physicalDevice, device,
                                         m_embeddingQueues);
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
    float priority, bool perferSperate) {
    auto index = m_physicalDevice.getPresentQueueFamily(m_surface);
    assert(index);

    m_embeddingQueues.queue[QueueType::Present].count++;
    m_embeddingQueues.queue[QueueType::Present].prioritys.push_back(priority);
    m_embeddingQueues.queue[QueueType::Present].familyIndex = index.value();
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireGraphicsQueue(
    float priority, bool perferSperate) {
    auto index = m_physicalDevice.getGraphicsQueueFamily(perferSperate);
    assert(index);
    m_embeddingQueues.queue[QueueType::Graphics].count++;
    m_embeddingQueues.queue[QueueType::Graphics].prioritys.push_back(priority);
    m_embeddingQueues.queue[QueueType::Graphics].familyIndex = index.value();
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireComputeQueue(
    float priority, bool perferSperate) {
    auto index = m_physicalDevice.getComputeQueueFamily(perferSperate);
    assert(index);
    m_embeddingQueues.queue[QueueType::Compute].count++;
    m_embeddingQueues.queue[QueueType::Compute].prioritys.push_back(priority);
    m_embeddingQueues.queue[QueueType::Compute].familyIndex = index.value();
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireTransferQueue(
    float priority, bool perferSperate) {
    auto index = m_physicalDevice.getTransferQueueFamily(perferSperate);
    assert(index);
    m_embeddingQueues.queue[QueueType::Transfer].count++;
    m_embeddingQueues.queue[QueueType::Transfer].prioritys.push_back(priority);
    m_embeddingQueues.queue[QueueType::Transfer].familyIndex = index.value();
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::requirePresent() {
    m_requirePresent = true;
    return *this;
}

std::vector<DeviceQueue> LogicDeviceBuilder::buildEmbeddingQueues() {
    std::vector<DeviceQueue> deviceQueues;
    for (auto i = 0; i < QueueType::Count; i++) {
        auto& queue = m_embeddingQueues.queue[i];
        if (queue.count <= 0) continue;
        deviceQueues.emplace_back(queue.familyIndex, queue.prioritys);
    }

    return deviceQueues;
}

END_VK_NAMESPACE
END_GFX_NAMESPACE