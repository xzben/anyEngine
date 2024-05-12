#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"

BEGIN_GFX_NAMESPACE

class Shader : public Handle, public GfxObject {
public:
    virtual bool build() = 0;

protected:
    Shader() : GfxObject(GfxObjectType::Shader) {}
    virtual ~Shader() = default;
};

END_GFX_NAMESPACE