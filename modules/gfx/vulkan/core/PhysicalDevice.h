#pragma once

#include <optional>
#include <vector>

#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class Instance;

class PhysicalDevice {
public:
    PhysicalDevice(VkPhysicalDevice physical_device);
    operator bool() const { return m_physicalDevice != VK_NULL_HANDLE; }
    operator VkPhysicalDevice() const { return m_physicalDevice; }

    PhysicalDevice(const PhysicalDevice&)            = delete;
    PhysicalDevice& operator=(const PhysicalDevice&) = delete;
    PhysicalDevice(PhysicalDevice&&);
    PhysicalDevice& operator=(PhysicalDevice&&);

public:
    const VkPhysicalDeviceFeatures& features() const { return m_features; }
    const VkPhysicalDeviceProperties& properties() const {
        return m_properties;
    }
    const VkPhysicalDeviceMemoryProperties& memoryProperties() const {
        return m_memoryProperties;
    }
    const std::vector<VkQueueFamilyProperties>& queueFamilyProperties() const {
        return m_queueFamiles;
    }

public:
    std::optional<uint32_t> getGraphicsQueueFamily(
        bool perferSeparate = true) const;
    std::optional<uint32_t> getComputeQueueFamily(
        bool perferSeparate = true) const;
    std::optional<uint32_t> getTransferQueueFamily(
        bool perferSeparate = true) const;
    std::optional<uint32_t> getPresentQueueFamily(VkSurfaceKHR surface) const;

private:
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};

    // The features that this GPU supports
    VkPhysicalDeviceFeatures m_features{};
    // The GPU properties
    VkPhysicalDeviceProperties m_properties;
    // The GPU memory properties
    VkPhysicalDeviceMemoryProperties m_memoryProperties;
    // The GPU queue family properties
    std::vector<VkQueueFamilyProperties> m_queueFamiles;
};

class PhysicalDeviceSelector {
public:
    explicit PhysicalDeviceSelector(const Instance* instance,
                                    VkSurfaceKHR surface = VK_NULL_HANDLE)
        : m_instance(instance), m_surface(surface) {}

    PhysicalDeviceSelector(const PhysicalDeviceSelector&)            = delete;
    PhysicalDeviceSelector(PhysicalDeviceSelector&&)                 = delete;
    PhysicalDeviceSelector& operator=(const PhysicalDeviceSelector&) = delete;
    PhysicalDeviceSelector& operator=(PhysicalDeviceSelector&&)      = delete;

    PhysicalDevice* select();

    PhysicalDeviceSelector& setSurface(VkSurfaceKHR surface);
    PhysicalDeviceSelector& preferDeviceType(VkPhysicalDeviceType type);
    PhysicalDeviceSelector& requirePresent();
    PhysicalDeviceSelector& requireTransferQueue(bool prefer_sperate);
    PhysicalDeviceSelector& requireComputeQueue(bool prefer_sperate);
    PhysicalDeviceSelector& requestDeviceMemorySize(VkDeviceSize size);
    PhysicalDeviceSelector& requireExtension(const char* extension);
    PhysicalDeviceSelector& requireFeatures(
        const VkPhysicalDeviceFeatures& features);

private:
    enum class Suitable { Full, Partial, No };
    Suitable isDeviceSuitable(PhysicalDevice& device);

private:
    const Instance* m_instance;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;

    VkPhysicalDeviceType m_preferDeviceType =
        VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

    // queues
    bool m_requirePresent               = false;
    bool m_requireTransferQueue         = false;
    bool m_requestSeparateTransferQueue = false;
    bool m_requireComputeQueue          = false;
    bool m_requestSeparateComputeQueue  = false;

    VkDeviceSize m_requestDeviceMemorySize = 0;
    std::vector<const char*> m_requireExtensions;
    VkPhysicalDeviceFeatures m_requireFeatures = {};
};

END_VK_NAMESPACE
END_GFX_NAMESPACE
