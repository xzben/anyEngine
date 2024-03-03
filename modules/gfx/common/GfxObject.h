#pragma once

#include <functional>

#include "gfx_types.h"

BEGIN_GFX_NAMESPACE

class GfxObject {
public:
    GfxObject(GfxObjectType type) : m_type(type){};
    virtual ~GfxObject() = default;

    GfxObject(const GfxObject &rhs)            = delete;
    GfxObject &operator=(const GfxObject &rhs) = delete;

    GfxObject(const GfxObject &&rhs)            = delete;
    GfxObject &operator=(const GfxObject &&rhs) = delete;

protected:
    GfxObjectType m_type{GfxObjectType::UnKnown};
};

END_GFX_NAMESPACE