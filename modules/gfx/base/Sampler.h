#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"

BEGIN_GFX_NAMESPACE

class Sampler : public Handle, public GfxObject {
public:
protected:
    Sampler() : GfxObject(GfxObjectType::Sampler) {}
    virtual ~Sampler() = default;
};

END_GFX_NAMESPACE