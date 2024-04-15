#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE
class CommandBuffer;

class CommandPool : public GfxObject {
public:
    virtual CommandBuffer* alloc(CommandBufferLevel level) = 0;
    virtual void free(CommandBuffer* buffer)               = 0;
    virtual void reset()                                   = 0;

protected:
    CommandPool() : GfxObject(GfxObjectType::CommandPool) {}
    virtual ~CommandPool() = default;
};
END_GFX_NAMESPACE