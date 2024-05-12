#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class RenderPass: public GfxObject {
public:
protected:
    RenderPass() : GfxObject(GfxObjectType::RenderPass) {}
    virtual ~RenderPass() = default;
};

END_GFX_NAMESPACE