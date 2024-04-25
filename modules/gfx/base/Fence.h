#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Fence : public Handle, public GfxObject {
public:
    virtual bool reset() = 0;
    /*
     *  timeout 单位 纳秒
     */
    virtual WaitResult wait(uint64_t timeout = 0) = 0;

protected:
    Fence() : GfxObject(GfxObjectType::Fence) {}
    virtual ~Fence() = default;
};
END_GFX_NAMESPACE