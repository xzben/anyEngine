#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Buffer : public Handle, public GfxObject {
public:
    virtual uint32_t size()     = 0;
    virtual uint32_t capacity() = 0;

protected:
    virtual void setSize(uint32_t size) = 0;

protected:
    Buffer() : GfxObject(GfxObjectType::Buffer) {}
    virtual ~Buffer() = default;
};

END_GFX_NAMESPACE