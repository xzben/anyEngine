#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Buffer {
public:
    Buffer()                                 = default;
    virtual ~Buffer()                        = default;
    virtual void update(const void* data, uint32_t size,
                        uint32_t offset = 0) = 0;

    virtual uint32_t size()     = 0;
    virtual uint32_t capacity() = 0;

    template <typename Handle>
    Handle getHandle() {
        return static_cast<Handle>(getHandleImp());
    }

protected:
    virtual GFX_HANDLE getHandleImp() = 0;
};

END_GFX_NAMESPACE