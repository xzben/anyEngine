#pragma once

#include "../base/CommandPool.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3CommandPool : public CommandPool {
public:
    GL3CommandPool(gl3::GLContext& context);
    virtual ~GL3CommandPool() {}
    virtual CommandBuffer* alloc(CommandBufferLevel level) override;
    virtual void reset() override;

private:
};

END_GFX_NAMESPACE