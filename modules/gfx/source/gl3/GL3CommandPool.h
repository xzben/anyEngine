#pragma once

#include "GL3CommandBuffer.h"
#include "base/CommandPool.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Queue;

class GL3CommandPool : public CommandPool {
public:
    GL3CommandPool(GL3Device& device, GL3Queue& queue, ResetMode resetModel);
    virtual ~GL3CommandPool();
    virtual GL3CommandBuffer* alloc(
        CommandBufferLevel level = CommandBufferLevel::PRIMARY) override;
    virtual void free(CommandBuffer* buffer) override;
    virtual void reset() override;

private:
    GL3Device& m_device;
    GL3Queue& m_queue;
    ResetMode m_resetModel;

    std::vector<GL3CommandBuffer*> m_buffers;
    uint32_t m_activeCount;
};

END_GFX_NAMESPACE