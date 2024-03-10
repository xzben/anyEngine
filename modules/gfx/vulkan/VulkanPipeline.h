#pragma once

#include "../base/Pipeline.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanPipeline : public Pipeline, public GfxObject {
private:
    const vk::LogicDevice& m_logicDevice;
};
END_GFX_NAMESPACE