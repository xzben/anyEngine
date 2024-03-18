#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Fence : public Handle {
public:
    Fence()              = default;
    virtual ~Fence()     = default;
    virtual bool reset() = 0;
    virtual bool wait(
        uint32_t timeout = std::numeric_limits<uint32_t>::max()) = 0;
};
END_GFX_NAMESPACE