#pragma once

#include <array>
#include <optional>

#include "Surface.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE

class VulkanQueue;
class VulkanCommandBuffer;
class VulkanCommandPool;

BEGIN_VK_NAMESPACE

struct QueueInfo {
    uint32_t familyIndex{0};
    std::vector<float> prioritys;
    uint32_t count{0};
};

struct EmbeddingQueues {
    QueueInfo queue[(int)QueueType::Count];
};
class PhysicalDevice;
class Instance;

class LogicDevice {
public:
    LogicDevice(const Instance& instance, const PhysicalDevice& physical_device,
                VkDevice device, EmbeddingQueues& embedding_queues);
    ~LogicDevice();

    const Instance& getInstance() const { return m_instance; }
    const PhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }

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
    const Instance& m_instance;
    const PhysicalDevice& m_physicalDevice;
    VkDevice m_device = VK_NULL_HANDLE;
    std::array<std::vector<VulkanQueue*>, (int)QueueType::Count>
        m_embeddingQueues;
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
    explicit LogicDeviceBuilder(const Instance& instance,
                                const PhysicalDevice& phycialDevice,
                                Surface& surface)
        : m_instance(instance),
          m_physicalDevice(phycialDevice),
          m_surface(surface) {}

    LogicDeviceBuilder(const LogicDeviceBuilder&)            = delete;
    LogicDeviceBuilder(LogicDeviceBuilder&&)                 = delete;
    LogicDeviceBuilder& operator=(const LogicDeviceBuilder&) = delete;
    LogicDeviceBuilder& operator=(LogicDeviceBuilder&&)      = delete;

    std::unique_ptr<LogicDevice> build();

    LogicDeviceBuilder& requireExtension(const char* extension);
    LogicDeviceBuilder& requireFeatures(
        const VkPhysicalDeviceFeatures& features);
    LogicDeviceBuilder& requireValidationLayer(const char* valid_layer);

    LogicDeviceBuilder& requirePresentQueue(float priority      = 1.0f,
                                            bool perfer_sperate = true);
    LogicDeviceBuilder& requireGraphicsQueue(float priority      = 1.0f,
                                             bool perfer_sperate = true);
    LogicDeviceBuilder& requireComputeQueue(float priority      = 1.0f,
                                            bool perfer_sperate = true);
    LogicDeviceBuilder& requireTransferQueue(float priority      = 1.0f,
                                             bool perfer_sperate = true);

    LogicDeviceBuilder& requireQueue(uint32_t queue_index,
                                     std::vector<float> priorities);
    LogicDeviceBuilder& requireQueue(uint32_t queue_index,
                                     float priorities = 1.f);

    LogicDeviceBuilder& requirePresent();

private:
    std::vector<DeviceQueue> buildEmbeddingQueues();

private:
    const Instance& m_instance;
    const PhysicalDevice& m_physicalDevice;
    const Surface& m_surface;

    bool m_requirePresent{true};
    VkPhysicalDeviceFeatures m_features = {};
    std::vector<const char*> m_extensions;
    std::vector<const char*> m_validationLayers;
    EmbeddingQueues m_embeddingQueues;
};
END_VK_NAMESPACE

END_GFX_NAMESPACE