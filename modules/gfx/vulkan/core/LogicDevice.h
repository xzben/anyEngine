#pragma once

#include <array>
#include <optional>

#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE

class VulkanQueue;
class VulkanCommandBuffer;
class VulkanCommandPool;

BEGIN_VK_NAMESPACE

struct QueueInfo {
    uint32_t familyIndex{0};
    uint32_t count{0};
};

struct EmbeddingQueues {
    QueueInfo queue[QueueType::Count];
};
class PhysicalDevice;
class Instance;

class LogicDevice {
public:
    LogicDevice(const Instance* instance, const PhysicalDevice* physical_device,
                VkDevice device, EmbeddingQueues& embedding_queues,
                std::vector<QueueInfo> custom_queues);
    ~LogicDevice();

    const Instance& instance() const { return *m_instance; }
    const PhysicalDevice& physical_device() const { return *m_physicalDevice; }
    void setSurface(VkSurfaceKHR surface) { m_surface = surface; }
    VkSurfaceKHR surface() { return m_surface; }

    operator bool() const { return m_device != VK_NULL_HANDLE; }
    operator VkDevice() const { return m_device; }

    void waitIdle() const;

    VmaAllocator getMemoryAllocator() const { return m_allocator; }

    std::optional<uint32_t> getQueueFamilyIndex(QueueType t) const;
    VulkanQueue* getQueue(QueueType t, int index = 0) const;
    VulkanCommandPool* createCommandPool(QueueType queue_type,
                                         ResetMode mode) const;
    VulkanCommandBuffer* requestCommandBuffer() const;

private:
    const Instance* m_instance;
    const PhysicalDevice* m_physicalDevice;

    VkDevice m_device      = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;

    std::array<std::vector<VulkanQueue*>, QueueType::Count> m_embeddingQueues;
    std::unordered_map<uint32_t, std::vector<VulkanQueue*>> m_customQueues;

    VulkanCommandPool* m_commandPool;
    VmaAllocator m_allocator = VK_NULL_HANDLE;
};

struct DeviceQueue {
    uint32_t familyIndex = 0;
    std::vector<float> priorities;

    DeviceQueue(uint32_t i, std::vector<float> p)
        : familyIndex(i), priorities(std::move(p)) {}
    DeviceQueue(uint32_t i, float p) : familyIndex(i), priorities{p} {}
};

class LogicDeviceBuilder {
public:
    explicit LogicDeviceBuilder(const Instance* instance,
                                const PhysicalDevice* phycialDevice,
                                VkSurfaceKHR surface)
        : m_instance(instance),
          m_physicalDevice(phycialDevice),
          m_surface(surface) {}

    LogicDeviceBuilder(const LogicDeviceBuilder&)            = delete;
    LogicDeviceBuilder(LogicDeviceBuilder&&)                 = delete;
    LogicDeviceBuilder& operator=(const LogicDeviceBuilder&) = delete;
    LogicDeviceBuilder& operator=(LogicDeviceBuilder&&)      = delete;

    LogicDevice* build();

    LogicDeviceBuilder& requireExtension(const char* extension);
    LogicDeviceBuilder& requireFeatures(
        const VkPhysicalDeviceFeatures& features);
    LogicDeviceBuilder& requireValidationLayer(const char* valid_layer);

    LogicDeviceBuilder& requirePresentQueue(uint32_t count      = 1,
                                            bool perfer_sperate = true);
    LogicDeviceBuilder& requireGraphicsQueue(uint32_t count      = 1,
                                             bool perfer_sperate = true);
    LogicDeviceBuilder& requireComputeQueue(uint32_t count      = 1,
                                            bool perfer_sperate = true);
    LogicDeviceBuilder& requireTransferQueue(uint32_t count      = 1,
                                             bool perfer_sperate = true);

    LogicDeviceBuilder& requireQueue(uint32_t queue_index,
                                     std::vector<float> priorities);
    LogicDeviceBuilder& requireQueue(uint32_t queue_index,
                                     float priorities = 1.f);

    LogicDeviceBuilder& requirePresent();

private:
    std::vector<DeviceQueue> buildEmbeddingQueues();

private:
    const Instance* m_instance;
    const PhysicalDevice* m_physicalDevice;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;

    bool m_requirePresent;
    VkPhysicalDeviceFeatures m_features = {};
    std::vector<const char*> m_extensions;
    std::vector<const char*> m_validationLayers;
    std::vector<DeviceQueue> m_customQueues;
    EmbeddingQueues m_embeddingQueues;
};
END_VK_NAMESPACE

END_GFX_NAMESPACE