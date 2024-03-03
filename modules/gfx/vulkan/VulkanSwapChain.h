#pragma once

#include "../base/SwapChain.h"
#include "core/core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanSwapChain : public SwapChain, public GfxObject<VkSwapchainKHR> {};
END_GFX_NAMESPACE