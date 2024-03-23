#include "VulkanTexture.h"

BEGIN_GFX_NAMESPACE

static uint32_t getLayercount(TextureType type) {
    switch (type) {
        case TextureType::CUBE:
            return 6;
    }

    return 1;
}
static VkImageCreateFlags getCreateFlag(TextureType type) {
    switch (type) {
        case TextureType::CUBE:
            return VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }

    return 0;
}

VulkanTexture::VulkanTexture(const vk::LogicDevice& device,
                             const TextureInfo& info)
    : GfxObject(GfxObjectType::Texture), m_logicDevice(device) {
    VkFormat vkFormat                 = vk::mapVkFormat(info.format);
    VkExtent3D extend                 = {info.width, info.height, 1};
    VkImageUsageFlags usage           = vk::mapVkImageUsage(info.usage);
    VkSampleCountFlagBits sampleCount = vk::mapVkSampleCount(info.sampleCount);
    int layercount                    = getLayercount(info.type);
    VkImageCreateFlags flags          = getCreateFlag(info.type);
    VkImageViewType viewType          = vk::mapVkImageViewType(info.type);

    m_image = std::make_unique<vk::Image>(
        m_logicDevice, extend, vkFormat, usage, VMA_MEMORY_USAGE_AUTO,
        sampleCount, info.miplevels, layercount, VK_IMAGE_TILING_OPTIMAL,
        flags);

    m_imageView = std::make_unique<vk::ImageView>(
        *m_image, viewType, vkFormat, 0, 0, info.miplevels, layercount);
}

END_GFX_NAMESPACE