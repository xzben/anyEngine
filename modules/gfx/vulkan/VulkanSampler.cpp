#include "VulkanSampler.h"

BEGIN_GFX_NAMESPACE

VulkanSampler::VulkanSampler(const vk::LogicDevice& device, SamplerInfo info)
    : m_logicDevice(device), m_info(info) {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter    = vk::mapVkFilter(m_info.magFilter);
    samplerInfo.minFilter    = vk::mapVkFilter(m_info.minFilter);
    samplerInfo.addressModeU = vk::mapVkSamplerAddressModel(m_info.addressU);
    samplerInfo.addressModeV = vk::mapVkSamplerAddressModel(m_info.addressV);
    samplerInfo.addressModeW = vk::mapVkSamplerAddressModel(m_info.addressW);
    samplerInfo.anisotropyEnable =
        m_info.maxAnisotropy > 0 ? VK_TRUE : VK_FALSE;
    samplerInfo.maxAnisotropy = static_cast<float>(m_info.maxAnisotropy);

    if (m_info.addressU == gfx::Address::BORDER
        || m_info.addressV == gfx::Address::BORDER
        || m_info.addressW == gfx::Address::BORDER) {
        samplerInfo.borderColor = vk::getBorderColor(m_info.bordercolor);
    } else {
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    }

    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    if (m_info.cmpFunc == gfx::CompareOp::EMPTY) {
        samplerInfo.compareEnable = VK_FALSE;
    } else {
        samplerInfo.compareEnable = VK_TRUE;
        samplerInfo.compareOp     = vk::mapVkCompareOp(m_info.cmpFunc);
    }

    samplerInfo.mipmapMode = vk::mapVkMipmapModel(m_info.mipFilter);
    samplerInfo.minLod     = 0.0f;
    samplerInfo.maxLod = VK_LOD_CLAMP_NONE;  // static_cast<float>(mipLevels);
    samplerInfo.mipLodBias = 0.0f;

    auto result = vkCreateSampler(device, &samplerInfo, nullptr, &m_handle);
    assert(result == VK_SUCCESS);
}

VulkanSampler::~VulkanSampler() {
    vkDestroySampler(m_logicDevice, m_handle, nullptr);
}

END_GFX_NAMESPACE