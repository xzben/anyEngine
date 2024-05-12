#include "ImageView.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

ImageView::ImageView(Image& image, VkImageViewType view_type, VkFormat format,
                     uint32_t base_mip_level, uint32_t base_array_layer,
                     uint32_t n_mip_levels, uint32_t n_array_layers)
    : m_image(&image),
      m_logicDevice(&image.getLogicDevice()),
      m_format(format) {
    if (m_format == VK_FORMAT_UNDEFINED) {
        m_format = image.format();
    }

    auto img_subresource = image.subresource();

    m_subresourceRange.baseMipLevel   = base_mip_level;
    m_subresourceRange.baseArrayLayer = base_array_layer;
    m_subresourceRange.levelCount =
        n_mip_levels == 0 ? img_subresource.mipLevel : n_mip_levels;
    m_subresourceRange.layerCount =
        n_array_layers == 0 ? img_subresource.arrayLayer : n_array_layers;

    if (isDepthOnlyFormat(m_format)) {
        m_subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    } else if (isDepthStencilFormat(m_format)) {
        m_subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    } else {
        m_subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    VkImageViewCreateInfo view_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view_info.image            = image;
    view_info.viewType         = view_type;
    view_info.format           = m_format;
    view_info.subresourceRange = m_subresourceRange;

    auto result =
        vkCreateImageView(*m_logicDevice, &view_info, nullptr, &m_handle);
    assert(result == VK_SUCCESS);
}

ImageView::ImageView(ImageView&& rhs)
    : m_logicDevice(rhs.m_logicDevice),
      m_image(rhs.m_image),
      m_format(rhs.m_format),
      m_subresourceRange(rhs.m_subresourceRange),
      m_handle(rhs.m_handle) {
    rhs.m_handle      = VK_NULL_HANDLE;
    rhs.m_image       = nullptr;
    rhs.m_logicDevice = nullptr;
}

ImageView& ImageView::operator=(ImageView&& rhs) {
    m_format           = rhs.m_format;
    m_handle           = rhs.m_handle;
    m_subresourceRange = rhs.m_subresourceRange;
    m_logicDevice      = rhs.m_logicDevice;
    m_image            = rhs.m_image;

    rhs.m_handle      = VK_NULL_HANDLE;
    rhs.m_image       = nullptr;
    rhs.m_logicDevice = nullptr;

    return *this;
}

ImageView::~ImageView() {
    if (m_handle != VK_NULL_HANDLE) {
        vkDestroyImageView(*m_logicDevice, m_handle, nullptr);
        m_handle = VK_NULL_HANDLE;
    }
}

END_VK_NAMESPACE
END_GFX_NAMESPACE