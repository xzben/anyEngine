#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class RenderPass {
public:
    RenderPass()          = default;
    virtual ~RenderPass() = default;
};

END_GFX_NAMESPACE