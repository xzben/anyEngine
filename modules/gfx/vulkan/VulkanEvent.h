#pragma once

#include "../base/Event.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanEvent : public Event, public GfxObject {
private:
    const vk::LogicDevice& m_logicDevice;
};
END_GFX_NAMESPACE