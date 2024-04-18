#pragma once

#include <atomic>
#include <mutex>

#include "../base/Queue.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Queue : public Queue {
public:
    GL3Queue(GL3Device& device, QueueType queueType, float priority);
    virtual ~GL3Queue();

    void destroyPool(CommandPool* pool) override;
    virtual CommandPool* createPool(
        ResetMode resetModel = ResetMode::ResetPool) override;
    virtual bool submit(const std::vector<CommandBuffer*>& cmd,
                        const std::vector<Semaphore*>& wait,
                        const std::vector<Semaphore*>& signal,
                        Fence* fence = nullptr) override;
    virtual uint32_t present(SwapChain* swapChain, uint32_t imageIndex,
                             const std::vector<Semaphore*>& waits) override;
    virtual bool waitIdle() override;
    virtual QueueType getType() override { return m_queueType; }

private:
    GL3Device& m_device;
    QueueType m_queueType;
    float m_priority;
};

END_GFX_NAMESPACE