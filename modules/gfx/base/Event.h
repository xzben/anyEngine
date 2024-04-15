#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Event : public GfxObject {
public:
protected:
    Event() : GfxObject(GfxObjectType::Event) {}
    virtual ~Event() = default;
};

END_GFX_NAMESPACE