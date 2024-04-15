#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Fence : public Handle, public GfxObject {
public:
    virtual bool reset()                    = 0;
    virtual bool wait(uint32_t timeout = 0) = 0;

protected:
    Fence() : GfxObject(GfxObjectType::Fence) {}
    virtual ~Fence() = default;
};
END_GFX_NAMESPACE