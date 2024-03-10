#pragma once

#include "../base/SwapChain.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanSwapChain : public SwapChain, public GfxObject {
private:
    VkSwapchainKHR m_handle{VK_NULL_HANDLE};
    const vk::LogicDevice& m_logicDevice;
};
END_GFX_NAMESPACE