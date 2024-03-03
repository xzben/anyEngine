#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE
class CommandBuffer;

class CommandPool {
public:
    virtual CommandBuffer* alloc()        = 0;
    virtual void free(CommandBuffer* cmd) = 0;
    virtual void reset()                  = 0;
};
END_GFX_NAMESPACE