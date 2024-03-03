#pragma once

#include "../base/Shader.h"
#include "core/core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanShader : public Shader, public GfxObject<VkQueue> {};
END_GFX_NAMESPACE