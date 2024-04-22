#pragma once

#include <atomic>
#include <mutex>

#include "../base/Queue.h"
#include "core/render/WorkTask.h"
#include "gl_common.h"
BEGIN_GFX_NAMESPACE
class GL3Device;

class GL3Queue : public Queue {
    friend class gl3::WorkTask;

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

protected:
    void handleTaskFinish(gl3::WorkTask* task);

private:
    GL3Device& m_device;
    QueueType m_queueType;
    float m_priority;

    std::atomic<uint32_t> m_workCount{0};
    std::condition_variable m_condition;
    std::mutex m_lock;
};

END_GFX_NAMESPACE