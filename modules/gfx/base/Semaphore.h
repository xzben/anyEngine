#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Semaphore {
public:
    Semaphore()          = default;
    virtual ~Semaphore() = default;
};
END_GFX_NAMESPACE