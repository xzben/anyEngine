#include "PhysicalDevice.h"

#include <algorithm>

#include "Instance.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

static bool supports_features(VkPhysicalDeviceFeatures supported,
                              VkPhysicalDeviceFeatures requested);

static std::optional<uint32_t> get_queue_family(
    const std::vector<VkQueueFamilyProperties>& families, VkQueueFlags with,
    VkQueueFlags without);

static std::optional<uint32_t> get_queue_family(
    const std::vector<VkQueueFamilyProperties>& families, bool perfer_separate,
    VkQueueFlags with, VkQueueFlags without1, VkQueueFlags without2);

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device) {
    vkGetPhysicalDeviceFeatures(physical_device, &m_features);
    vkGetPhysicalDeviceProperties(physical_device, &m_properties);
    vkGetPhysicalDeviceMemoryProperties(physical_device, &m_memoryProperties);

    uint32_t queue_family_properties_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        physical_device, &queue_family_properties_count, nullptr);
    m_queueFamiles.resize(queue_family_properties_count);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physical_device, &queue_family_properties_count, m_queueFamiles.data());
}

PhysicalDevice::PhysicalDevice(PhysicalDevice&& other) {
    *this = std::move(other);

    other.m_physicalDevice = VK_NULL_HANDLE;
}

PhysicalDevice& PhysicalDevice::operator=(PhysicalDevice&& other) {
    m_physicalDevice   = other.m_physicalDevice;
    m_features         = other.m_features;
    m_properties       = other.m_properties;
    m_memoryProperties = other.m_memoryProperties;
    m_queueFamiles     = std::move(other.m_queueFamiles);

    other.m_physicalDevice = VK_NULL_HANDLE;
    return *this;
}

std::optional<uint32_t> PhysicalDevice::getGraphicsQueueFamily(
    bool perferSeparate) const {
    return get_queue_family(m_queueFamiles, perferSeparate,
                            VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT,
                            VK_QUEUE_COMPUTE_BIT);
}

std::optional<uint32_t> PhysicalDevice::getComputeQueueFamily(
    bool perferSeparate) const {
    return get_queue_family(m_queueFamiles, perferSeparate,
                            VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT,
                            VK_QUEUE_GRAPHICS_BIT);
}

std::optional<uint32_t> PhysicalDevice::getTransferQueueFamily(
    bool perferSeparate) const {
    return get_queue_family(m_queueFamiles, perferSeparate,
                            VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT,
                            VK_QUEUE_COMPUTE_BIT);
}

std::optional<uint32_t> PhysicalDevice::getPresentQueueFamily(
    VkSurfaceKHR surface) const {
    for (auto i = 0; i < m_queueFamiles.size(); ++i) {
        VkBool32 supported = VK_FALSE;
        auto result = vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i,
                                                           surface, &supported);
        if (result == VK_SUCCESS && supported == VK_TRUE) {
            return i;
        }
    }

    return std::nullopt;
}

// clang-format on
static std::optional<uint32_t> get_queue_family(
    const std::vector<VkQueueFamilyProperties>& families, VkQueueFlags with,
    VkQueueFlags without) {
    for (auto i = 0; i < families.size(); ++i) {
        if ((families[i].queueFlags & with) == with
            && !(families[i].queueFlags & without)) {
            return i;
        }
    }

    return std::nullopt;
}

static std::optional<uint32_t> get_queue_family(
    const std::vector<VkQueueFamilyProperties>& families, bool perfer_separate,
    VkQueueFlags with, VkQueueFlags without1, VkQueueFlags without2) {
    if (perfer_separate) {
        if (auto index = get_queue_family(families, with, without1 | without2);
            index) {
            return index;
        }

        if (auto index = get_queue_family(families, with, without1); index) {
            return index;
        }

        if (auto index = get_queue_family(families, with, without2); index) {
            return index;
        }
    }

    return get_queue_family(families, with, {});
}

PhysicalDevice PhysicalDeviceSelector::select() {
    if (m_requirePresent && !m_surface) {
        return nullptr;
    }

    const auto& allDevices = m_instance.getPhysicalDevices();

    std::vector<std::pair<Suitable, PhysicalDevice>> avalidDevices;
    for (const auto& device : allDevices) {
        auto suitable = isDeviceSuitable(device);
        if (suitable != Suitable::No) {
            avalidDevices.push_back(std::make_pair(suitable, (VkPhysicalDevice)device));
        }
    }
    if (avalidDevices.empty()) {
        return nullptr;
    }

    std::stable_partition(
        avalidDevices.begin(), avalidDevices.end(),
        [](auto const& p) { return p.first == Suitable::Full; });

    if (avalidDevices[0].first == Suitable::No) {
        return nullptr;
    }

    return std::move(avalidDevices[0].second);
}

PhysicalDeviceSelector& PhysicalDeviceSelector::setSurface(
    VkSurfaceKHR surface) {
    m_surface = surface;

    return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::preferDeviceType(
    VkPhysicalDeviceType type) {
    m_preferDeviceType = type;
    return *this;
}
PhysicalDeviceSelector& PhysicalDeviceSelector::requirePresent() {
    m_requirePresent = true;
    return *this;
}
PhysicalDeviceSelector& PhysicalDeviceSelector::requireTransferQueue(
    bool preferSperate) {
    m_requireTransferQueue         = true;
    m_requestSeparateTransferQueue = preferSperate;

    return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requireComputeQueue(
    bool preferSperate) {
    m_requireComputeQueue         = true;
    m_requestSeparateComputeQueue = preferSperate;
    return *this;
}
PhysicalDeviceSelector& PhysicalDeviceSelector::requestDeviceMemorySize(
    VkDeviceSize size) {
    m_requestDeviceMemorySize = size;
    return *this;
}
PhysicalDeviceSelector& PhysicalDeviceSelector::requireExtension(
    const char* extension) {
    m_requireExtensions.push_back(extension);

    return *this;
}
PhysicalDeviceSelector& PhysicalDeviceSelector::requireFeatures(
    const VkPhysicalDeviceFeatures& features) {
    m_requireFeatures = features;
    return *this;
}
PhysicalDeviceSelector::Suitable PhysicalDeviceSelector::isDeviceSuitable(
    const PhysicalDevice& device) {
    Suitable suitable = Suitable::Full;

    // extension ok ?
    uint32_t ext_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &ext_count, nullptr);
    std::vector<VkExtensionProperties> extensions(ext_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &ext_count,
                                         extensions.data());

    auto is_extension_support = [&extensions](const char* ext) {
        return std::find_if(extensions.begin(), extensions.end(),
                            [ext](VkExtensionProperties& p) {
                                return std::strcmp(p.extensionName, ext) == 0;
                            })
               != extensions.end();
    };
    for (const char* ext : m_requireExtensions) {
        if (!is_extension_support(ext)) {
            return Suitable::No;
        }
    }

    // features
    if (!supports_features(device.features(), m_requireFeatures)) {
        return Suitable::No;
    }

    // queue family
    bool has_separate_transfer_queue = false;
    bool has_transfer_queue          = false;
    bool has_separate_compute_queue  = false;
    bool has_compute_queue           = false;
    bool has_present_queue           = false;

    auto& families = device.queueFamilyProperties();
    for (auto i = 0; i < families.size(); ++i) {
        if ((families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)) {
            has_transfer_queue = true;
            if (!(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                has_separate_transfer_queue = true;
            }
        }

        if ((families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
            has_compute_queue = true;
            if (!(families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)) {
                has_separate_compute_queue = true;
            }
        }

        VkBool32 supported = VK_FALSE;

        auto result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface,
                                                           &supported);
        if (result == VK_SUCCESS && supported == VK_TRUE) {
            has_present_queue = true;
        }
    }

    if (m_requireTransferQueue && !has_transfer_queue) {
        return Suitable::No;
    }

    if (m_requireComputeQueue && !has_compute_queue) {
        return Suitable::No;
    }

    if (m_requestSeparateTransferQueue && !has_separate_transfer_queue) {
        suitable = Suitable::Partial;
    }

    if (m_requestSeparateComputeQueue && !has_separate_compute_queue) {
        suitable = Suitable::Partial;
    }

    // present
    if (m_requirePresent) {
        if (!has_present_queue) {
            return Suitable::No;
        }

        if (!is_extension_support(VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
            return Suitable::No;
        }

        uint32_t mode_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface,
                                                  &mode_count, nullptr);
        if (mode_count == 0) {
            return Suitable::No;
        }
    }

    // device type
    auto& properties = device.properties();
    if (properties.deviceType != m_preferDeviceType) {
        suitable = Suitable::Partial;
    }

    // memory
    auto& memory_properties = device.memoryProperties();
    for (uint32_t i = 0; i < memory_properties.memoryHeapCount; i++) {
        if (memory_properties.memoryHeaps[i].flags
            & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            if (memory_properties.memoryHeaps[i].size
                < m_requestDeviceMemorySize) {
                return Suitable::Partial;
            }
        }
    }

    return suitable;
}
// clang-format off
static bool supports_features(VkPhysicalDeviceFeatures supported,
					   VkPhysicalDeviceFeatures requested) {
	if (requested.robustBufferAccess && !supported.robustBufferAccess) return false;
	if (requested.fullDrawIndexUint32 && !supported.fullDrawIndexUint32) return false;
	if (requested.imageCubeArray && !supported.imageCubeArray) return false;
	if (requested.independentBlend && !supported.independentBlend) return false;
	if (requested.geometryShader && !supported.geometryShader) return false;
	if (requested.tessellationShader && !supported.tessellationShader) return false;
	if (requested.sampleRateShading && !supported.sampleRateShading) return false;
	if (requested.dualSrcBlend && !supported.dualSrcBlend) return false;
	if (requested.logicOp && !supported.logicOp) return false;
	if (requested.multiDrawIndirect && !supported.multiDrawIndirect) return false;
	if (requested.drawIndirectFirstInstance && !supported.drawIndirectFirstInstance) return false;
	if (requested.depthClamp && !supported.depthClamp) return false;
	if (requested.depthBiasClamp && !supported.depthBiasClamp) return false;
	if (requested.fillModeNonSolid && !supported.fillModeNonSolid) return false;
	if (requested.depthBounds && !supported.depthBounds) return false;
	if (requested.wideLines && !supported.wideLines) return false;
	if (requested.largePoints && !supported.largePoints) return false;
	if (requested.alphaToOne && !supported.alphaToOne) return false;
	if (requested.multiViewport && !supported.multiViewport) return false;
	if (requested.samplerAnisotropy && !supported.samplerAnisotropy) return false;
	if (requested.textureCompressionETC2 && !supported.textureCompressionETC2) return false;
	if (requested.textureCompressionASTC_LDR && !supported.textureCompressionASTC_LDR) return false;
	if (requested.textureCompressionBC && !supported.textureCompressionBC) return false;
	if (requested.occlusionQueryPrecise && !supported.occlusionQueryPrecise) return false;
	if (requested.pipelineStatisticsQuery && !supported.pipelineStatisticsQuery) return false;
	if (requested.vertexPipelineStoresAndAtomics && !supported.vertexPipelineStoresAndAtomics) return false;
	if (requested.fragmentStoresAndAtomics && !supported.fragmentStoresAndAtomics) return false;
	if (requested.shaderTessellationAndGeometryPointSize && !supported.shaderTessellationAndGeometryPointSize) return false;
	if (requested.shaderImageGatherExtended && !supported.shaderImageGatherExtended) return false;
	if (requested.shaderStorageImageExtendedFormats && !supported.shaderStorageImageExtendedFormats) return false;
	if (requested.shaderStorageImageMultisample && !supported.shaderStorageImageMultisample) return false;
	if (requested.shaderStorageImageReadWithoutFormat && !supported.shaderStorageImageReadWithoutFormat) return false;
	if (requested.shaderStorageImageWriteWithoutFormat && !supported.shaderStorageImageWriteWithoutFormat) return false;
	if (requested.shaderUniformBufferArrayDynamicIndexing && !supported.shaderUniformBufferArrayDynamicIndexing) return false;
	if (requested.shaderSampledImageArrayDynamicIndexing && !supported.shaderSampledImageArrayDynamicIndexing) return false;
	if (requested.shaderStorageBufferArrayDynamicIndexing && !supported.shaderStorageBufferArrayDynamicIndexing) return false;
	if (requested.shaderStorageImageArrayDynamicIndexing && !supported.shaderStorageImageArrayDynamicIndexing) return false;
	if (requested.shaderClipDistance && !supported.shaderClipDistance) return false;
	if (requested.shaderCullDistance && !supported.shaderCullDistance) return false;
	if (requested.shaderFloat64 && !supported.shaderFloat64) return false;
	if (requested.shaderInt64 && !supported.shaderInt64) return false;
	if (requested.shaderInt16 && !supported.shaderInt16) return false;
	if (requested.shaderResourceResidency && !supported.shaderResourceResidency) return false;
	if (requested.shaderResourceMinLod && !supported.shaderResourceMinLod) return false;
	if (requested.sparseBinding && !supported.sparseBinding) return false;
	if (requested.sparseResidencyBuffer && !supported.sparseResidencyBuffer) return false;
	if (requested.sparseResidencyImage2D && !supported.sparseResidencyImage2D) return false;
	if (requested.sparseResidencyImage3D && !supported.sparseResidencyImage3D) return false;
	if (requested.sparseResidency2Samples && !supported.sparseResidency2Samples) return false;
	if (requested.sparseResidency4Samples && !supported.sparseResidency4Samples) return false;
	if (requested.sparseResidency8Samples && !supported.sparseResidency8Samples) return false;
	if (requested.sparseResidency16Samples && !supported.sparseResidency16Samples) return false;
	if (requested.sparseResidencyAliased && !supported.sparseResidencyAliased) return false;
	if (requested.variableMultisampleRate && !supported.variableMultisampleRate) return false;
	if (requested.inheritedQueries && !supported.inheritedQueries) return false;

    return true;
}


END_VK_NAMESPACE
END_GFX_NAMESPACE