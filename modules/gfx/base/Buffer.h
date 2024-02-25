#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Buffer {
public:
    virtual void update(const void* data, uint32_t size,
                        uint32_t offset = 0)              = 0;
    virtual void flush()                                  = 0;
    virtual void* mapData(uint32_t offset, uint32_t size) = 0;
};

END_GFX_NAMESPACE