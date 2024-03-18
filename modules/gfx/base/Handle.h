#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Handle {
public:
    template <typename Handle>
    Handle getHandle() const {
        return static_cast<Handle>(getHandleImp());
    }

protected:
    virtual GFX_HANDLE getHandleImp() const = 0;
};
END_GFX_NAMESPACE