#pragma once

#include "../base/Sampler.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanSampler : public Sampler, public GfxObject {
public:
    VulkanSampler(const vk::LogicDevice& device, SamplerInfo info);
    virtual ~VulkanSampler();
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkSampler() const { return m_handle; }

private:
    const vk::LogicDevice& m_logicDevice;
    VkSampler m_handle{VK_NULL_HANDLE};
    SamplerInfo m_info;
};
END_GFX_NAMESPACE