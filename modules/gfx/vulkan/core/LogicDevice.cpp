#include "LogicDevice.h"

#include "../VulkanQueue.h"
#include "Instance.h"
#include "PhysicalDevice.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

LogicDevice::LogicDevice(const Instance* instance,
                         const PhysicalDevice* physical_device, VkDevice device,
                         EmbeddingQueues& embedding_queues,
                         std::vector<QueueInfo> custom_queues)
    : m_instance(instance),
      m_physicalDevice(physical_device),
      m_device(device) {
    // for (auto i = 0; i < QueueType::Count; ++i) {
    //     auto& info = embedding_queues.queue[i];
    //     for (auto j = 0; j < info.count; ++j) {
    //         m_embeddingQueues[i].emplace_back(*this, info.family_index, j);
    //     }
    // }

    // for (auto& info : custom_queues) {
    //     std::vector<Queue> queues;
    //     queues.reserve(info.count);
    //     for (auto i = 0; i < info.count; ++i) {
    //         queues.emplace_back(*this, info.family_index, i);
    //     }
    //     m_customQueues.emplace(info.family_index, std::move(queues));
    // }

    // TODO: use reset pool for performance
    m_commandPool =
        createCommandPool(QueueType::Graphics, ResetMode::AlwaysAllocate);

    // vma
    VmaAllocatorCreateInfo vma_info = {};
    vma_info.physicalDevice         = *m_physicalDevice;
    vma_info.device                 = m_device;
    vma_info.instance               = *m_instance;
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

    if (m_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(*m_instance, m_surface, nullptr);
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
    if (auto index = getQueueFamilyIndex(queue_type); index) {
        // return create_command_pool(index.value(), mode);
    }

    return nullptr;
}

VulkanCommandBuffer* LogicDevice::requestCommandBuffer() const {
    // return _command_pool->request_command_buffer();
    return nullptr;
}
LogicDevice* LogicDeviceBuilder::build() { return nullptr; }

LogicDeviceBuilder& LogicDeviceBuilder::requireExtension(
    const char* extension) {
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireFeatures(
    const VkPhysicalDeviceFeatures& features) {
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireValidationLayer(
    const char* valid_layer) {
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::requirePresentQueue(
    uint32_t count, bool perferSperate) {
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireGraphicsQueue(
    uint32_t count, bool perferSperate) {
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireComputeQueue(
    uint32_t count, bool perferSperate) {
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireTransferQueue(
    uint32_t count, bool perferSperate) {
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::requireQueue(
    uint32_t queue_index, std::vector<float> priorities) {
    return *this;
}
LogicDeviceBuilder& LogicDeviceBuilder::requireQueue(uint32_t queue_index,
                                                     float priorities) {
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::requirePresent() { return *this; }

std::vector<DeviceQueue> LogicDeviceBuilder::buildEmbeddingQueues() {
    return {};
}

END_VK_NAMESPACE
END_GFX_NAMESPACE