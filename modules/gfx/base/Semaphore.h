#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"

BEGIN_GFX_NAMESPACE

class Semaphore : public Handle {
public:
    Semaphore()          = default;
    virtual ~Semaphore() = default;
};
END_GFX_NAMESPACE