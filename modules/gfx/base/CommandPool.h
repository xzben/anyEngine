#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE
class CommandBuffer;

class CommandPool {
public:
    CommandPool()                                          = default;
    virtual ~CommandPool()                                 = default;
    virtual CommandBuffer* alloc(CommandBufferLevel level) = 0;
    virtual void reset()                                   = 0;
};
END_GFX_NAMESPACE