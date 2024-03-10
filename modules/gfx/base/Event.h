#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Event {
public:
    Event()          = default;
    virtual ~Event() = default;
};

END_GFX_NAMESPACE