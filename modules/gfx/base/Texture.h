#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Texture : public GfxObject {
public:
    template <typename Handle>
    Handle getHandle() {
        return static_cast<Handle>(getHandleImp());
    }
    virtual const TextureInfo& getInfo() = 0;

protected:
    Texture() : GfxObject(GfxObjectType::Texture) {}
    virtual ~Texture()                = default;
    virtual GFX_HANDLE getHandleImp() = 0;
};

END_GFX_NAMESPACE