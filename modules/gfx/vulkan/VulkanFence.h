#pragma once

#include "../base/Fence.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanFence : public Fence, public GfxObject {
private:
    const vk::LogicDevice& m_logicDevice;
};
END_GFX_NAMESPACE