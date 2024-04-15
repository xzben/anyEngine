#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"

BEGIN_GFX_NAMESPACE

class Semaphore : public Handle, public GfxObject {
public:
    virtual void wait()   = 0;
    virtual void signal() = 0;

protected:
    Semaphore() : GfxObject(GfxObjectType::Semaphore) {}
    virtual ~Semaphore() = default;
};
END_GFX_NAMESPACE