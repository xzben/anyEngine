#include "Image.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

static inline VkImageType findImageType(VkExtent3D extent);

Image::Image(const LogicDevice& device, VkImage handle,
             const VkExtent3D& extent, VkFormat format,
             VkImageUsageFlags image_usage, VkSampleCountFlagBits sample_count)
    : m_logicDevice(&device),
      m_handle(handle),
      m_type(findImageType(extent)),
      m_extent(extent),
      m_format(format),
      m_usage(image_usage),
      m_sampleCount(sample_count) {}

Image::Image(const LogicDevice& device, const VkExtent3D& extent,
             VkFormat format, VkImageUsageFlags image_usage,
             VmaMemoryUsage memory_usage, VkSampleCountFlagBits sample_count,
             uint32_t mip_levels, uint32_t array_layers, VkImageTiling tiling,
             VkImageCreateFlags flags,
             const std::vector<uint32_t>& queue_family_indices)
    : m_logicDevice(&device),
      m_handle(VK_NULL_HANDLE),
      m_type(findImageType(extent)),
      m_extent(extent),
      m_format(format),
      m_usage(image_usage),
      m_sampleCount(sample_count),
      m_tiling(tiling),
      m_arrayLayerCount(array_layers) {
    assert(mip_levels > 0);
    assert(array_layers > 0);

    if (isDepthOnlyFormat(format)) {
        m_subresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    } else if (isDepthStencilFormat(format)) {
        m_subresource.aspectMask =
            VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    } else {
        m_subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    m_subresource.mipLevel   = mip_levels;
    m_subresource.arrayLayer = array_layers;

    VkImageCreateInfo image_info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    image_info.flags       = flags;
    image_info.imageType   = m_type;
    image_info.format      = m_format;
    image_info.extent      = m_extent;
    image_info.mipLevels   = mip_levels;
    image_info.arrayLayers = m_arrayLayerCount;
    image_info.samples     = m_sampleCount;
    image_info.tiling      = m_tiling;
    image_info.usage       = image_usage;

    if (queue_family_indices.size() >= 2) {
        image_info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
        image_info.queueFamilyIndexCount = queue_family_indices.size();
        image_info.pQueueFamilyIndices   = queue_family_indices.data();
    }

    VmaAllocationCreateInfo memory_info{};
    memory_info.usage = memory_usage;

    if (image_usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
        memory_info.preferredFlags = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
    }

    auto result =
        vmaCreateImage(m_logicDevice->getMemoryAllocator(), &image_info,
                       &memory_info, &m_handle, &m_allocation, nullptr);

    assert(result == VK_SUCCESS);
}

Image::Image(Image&& other)
    : m_logicDevice(other.m_logicDevice),
      m_handle(other.m_handle),
      m_allocation(other.m_allocation),
      m_type(other.m_type),
      m_extent(other.m_extent),
      m_format(other.m_format),
      m_usage(other.m_usage),
      m_sampleCount(other.m_sampleCount),
      m_tiling(other.m_tiling),
      m_subresource(other.m_subresource),
      m_arrayLayerCount(other.m_arrayLayerCount),
      m_mappedData(other.m_mappedData) {
    other.m_allocation  = VK_NULL_HANDLE;
    other.m_handle      = VK_NULL_HANDLE;
    other.m_logicDevice = nullptr;
}

Image& Image::operator=(Image&& other) {
    if (m_mappedData) {
        CCERROR("moved out with mapping");
        unmap();
    }

    if (m_allocation) {
        vmaDestroyImage(m_logicDevice->getMemoryAllocator(), m_handle,
                        m_allocation);
    }

    m_allocation      = other.m_allocation;
    m_type            = other.m_type;
    m_extent          = other.m_extent;
    m_format          = other.m_format;
    m_usage           = other.m_usage;
    m_sampleCount     = other.m_sampleCount;
    m_tiling          = other.m_tiling;
    m_subresource     = other.m_subresource;
    m_arrayLayerCount = other.m_arrayLayerCount;
    m_mappedData      = other.m_mappedData;
    m_logicDevice     = other.m_logicDevice;
    m_handle          = other.m_handle;

    other.m_logicDevice = nullptr;
    other.m_allocation  = VK_NULL_HANDLE;
    other.m_handle      = VK_NULL_HANDLE;

    return *this;
}

Image::~Image() {
    if (m_allocation) {
        vmaDestroyImage(m_logicDevice->getMemoryAllocator(), m_handle,
                        m_allocation);
        m_allocation = VK_NULL_HANDLE;
    }
}

void* Image::map() {
    if (!m_mappedData) {
        if (m_tiling != VK_IMAGE_TILING_LINEAR) {
            CCERROR("Mapping image memory that is not linear");
        }
        vmaMapMemory(m_logicDevice->getMemoryAllocator(), m_allocation,
                     &m_mappedData);
    }
    return m_mappedData;
}

void Image::unmap() {
    if (m_mappedData) {
        vmaUnmapMemory(m_logicDevice->getMemoryAllocator(), m_allocation);
        m_mappedData = nullptr;
    }
}

static inline VkImageType findImageType(VkExtent3D extent) {
    VkImageType result{};

    uint32_t dim_num = 0;
    if (extent.width >= 1) {
        dim_num++;
    }
    if (extent.height >= 1) {
        dim_num++;
    }
    if (extent.depth > 1) {
        dim_num++;
    }

    switch (dim_num) {
        case 1:
            return VK_IMAGE_TYPE_1D;
            break;
        case 2:
            return VK_IMAGE_TYPE_2D;
            break;
        case 3:
            return VK_IMAGE_TYPE_3D;
            break;
        default:
            throw std::runtime_error("No image type found.");
            break;
    }

    return result;
}
END_VK_NAMESPACE
END_GFX_NAMESPACE