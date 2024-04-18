#pragma once

#include "../base/CommandPool.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Queue;

class GL3CommandPool : public CommandPool {
public:
    GL3CommandPool(GL3Device& device, GL3Queue& queue);
    virtual ~GL3CommandPool();
    virtual CommandBuffer* alloc(CommandBufferLevel level) override;
    virtual void free(CommandBuffer* buffer) override;
    virtual void reset() override;

private:
    GL3Device& m_device;
    GL3Queue& m_queue;
};

END_GFX_NAMESPACE