#pragma once

#include "Image.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class ImageView {
public:
    ImageView() = default;
    ImageView(Image& image, VkImageViewType view_type,
              VkFormat format         = VK_FORMAT_UNDEFINED,
              uint32_t base_mip_level = 0, uint32_t base_array_layer = 0,
              uint32_t n_mip_levels = 1, uint32_t n_array_layers = 1);

    ImageView(ImageView&&);
    ImageView& operator=(ImageView&&);

    virtual ~ImageView();

public:
    VkFormat format() const { return m_format; }
    VkImageSubresourceRange subresourceRange() const {
        return m_subresourceRange;
    }
    VkImageSubresourceLayers subresourceLayers() const;

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkImageView() const { return m_handle; }

private:
    const LogicDevice* m_logicDevice{nullptr};
    const Image* m_image;
    VkFormat m_format{};
    VkImageSubresourceRange m_subresourceRange{};
    VkImageView m_handle{VK_NULL_HANDLE};
};
END_VK_NAMESPACE
END_GFX_NAMESPACE
