#pragma once

#include "../base/Queue.h"
#include "core/core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanQueue : public Queue, public GfxObject {
public:
    uint32_t familyIndex() { return 0; }
};
END_GFX_NAMESPACE