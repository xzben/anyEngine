#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Sampler: public GfxObject {
public:
protected:
    Sampler() : GfxObject(GfxObjectType::Sampler) {}
    virtual ~Sampler() = default;
};

END_GFX_NAMESPACE