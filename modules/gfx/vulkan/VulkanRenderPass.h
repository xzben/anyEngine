#pragma once

#include "../base/RenderPass.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanRenderPass : public RenderPass, public GfxObject {
private:
    const vk::LogicDevice& m_logicDevice;
};
END_GFX_NAMESPACE