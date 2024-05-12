#pragma once

#include "LogicDevice.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class Image {
public:
    Image() = default;
    Image(const LogicDevice& device, VkImage handle, const VkExtent3D& extent,
          VkFormat format, VkImageUsageFlags image_usage,
          VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT);

    Image(const LogicDevice& device, const VkExtent3D& extent, VkFormat format,
          VkImageUsageFlags image_usage,
          VmaMemoryUsage memory_usage        = VMA_MEMORY_USAGE_AUTO,
          VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT,
          uint32_t mip_levels = 1, uint32_t array_layers = 1,
          VkImageTiling tiling     = VK_IMAGE_TILING_OPTIMAL,
          VkImageCreateFlags flags = 0,
          const std::vector<uint32_t>& queue_family_indices = {});

    Image(Image&&);
    Image& operator=(Image&&);

    ~Image();

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkImage() const { return m_handle; }
    VkImage getHandle() const { return m_handle; }
    const LogicDevice& getLogicDevice() const { return *m_logicDevice; }

public:
    void* map();
    void unmap();

    VkImageType type() const { return m_type; }
    const VkExtent3D& extent() const { return m_extent; }
    VkFormat format() const { return m_format; }
    VkSampleCountFlagBits sampleCount() const { return m_sampleCount; }
    VkImageUsageFlags usage() const { return m_usage; }
    VkImageTiling tiling() const { return m_tiling; }
    VkImageSubresource subresource() const { return m_subresource; }
    uint32_t arrayLayerCount() const { return m_arrayLayerCount; }

private:
    const LogicDevice* m_logicDevice{nullptr};
    VkImage m_handle{VK_NULL_HANDLE};
    VmaAllocation m_allocation = VK_NULL_HANDLE;
    VkImageType m_type{};
    VkExtent3D m_extent{};
    VkFormat m_format{};
    VkImageUsageFlags m_usage{};
    VkSampleCountFlagBits m_sampleCount{};
    VkImageTiling m_tiling{};
    VkImageSubresource m_subresource{};
    uint32_t m_arrayLayerCount{0};
    void* m_mappedData{nullptr};
};

END_VK_NAMESPACE
END_GFX_NAMESPACE