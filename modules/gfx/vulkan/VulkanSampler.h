#pragma once

#include "../base/Sampler.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanSampler : public Sampler, public GfxObject {
private:
    const vk::LogicDevice& m_logicDevice;
};
END_GFX_NAMESPACE