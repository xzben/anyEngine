#pragma once

#include "../base/CommandPool.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3CommandPool : public CommandPool {
public:
    GL3CommandPool(GL3Device& device);
    virtual ~GL3CommandPool();
    virtual CommandBuffer* alloc(CommandBufferLevel level) override;
    virtual void reset() override;

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE