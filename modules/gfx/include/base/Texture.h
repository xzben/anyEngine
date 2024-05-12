#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Texture : public Handle, public GfxObject {
public:
    virtual const TextureInfo& getInfo() const = 0;

protected:
    Texture() : GfxObject(GfxObjectType::Texture) {}
    virtual ~Texture() = default;
};

END_GFX_NAMESPACE