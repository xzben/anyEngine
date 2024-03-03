#pragma once

#include "../base/Texture.h"
#include "core/core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanTexture : public Texture, public GfxObject<VkImageView> {};
END_GFX_NAMESPACE