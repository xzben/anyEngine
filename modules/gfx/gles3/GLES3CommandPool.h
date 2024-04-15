#pragma once

#include "../base/CommandPool.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3CommandPool : public CommandPool {
public:
    GLES3CommandPool(gles::GLContext& context);
    virtual ~GLES3CommandPool() {}
    virtual CommandBuffer* alloc(CommandBufferLevel level) override;
    virtual void reset() override;

private:
};

END_GFX_NAMESPACE