#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class SwapChain {
public:
    SwapChain()          = default;
    virtual ~SwapChain() = default;
};

END_GFX_NAMESPACE