#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Texture {
public:
    Texture()          = default;
    virtual ~Texture() = default;

    template <typename Handle>
    Handle getHandle() {
        return static_cast<Handle>(getHandleImp());
    }

    virtual const SizeI& getSize() = 0;

protected:
    virtual GFX_HANDLE getHandleImp() = 0;
};

END_GFX_NAMESPACE