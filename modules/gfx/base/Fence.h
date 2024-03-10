#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Fence {
public:
    Fence()          = default;
    virtual ~Fence() = default;
    bool reset();
    bool wait(uint32_t timeout = std::numeric_limits<uint32_t>::max());
};
END_GFX_NAMESPACE