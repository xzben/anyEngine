#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Buffer : public Handle {
public:
    Buffer()                                 = default;
    virtual ~Buffer()                        = default;
    virtual void update(const void* data, uint32_t size,
                        uint32_t offset = 0) = 0;

    virtual uint32_t size()     = 0;
    virtual uint32_t capacity() = 0;
};

END_GFX_NAMESPACE