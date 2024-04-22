#pragma once

#include "../gl3_core_common.h"

BEGIN_GFX_NAMESPACE
class CommandBuffer;
class Semaphore;
class Fence;
class SwapChain;
class GL3Queue;

BEGIN_GL3_CORE_NAMESPACE

class GLContext;

class WorkTask {
public:
    WorkTask(GL3Queue* queue) : m_queue(queue) {}
    virtual ~WorkTask()                      = default;
    virtual void execute(GLContext* context) = 0;

    void finish();

protected:
    GL3Queue* m_queue{nullptr};
};

class RenderWorkTask : public WorkTask {
public:
    RenderWorkTask(GL3Queue* queue, const std::vector<CommandBuffer*>& cmd,
                   const std::vector<Semaphore*>& wait,
                   const std::vector<Semaphore*>& signal, Fence* fence);
    virtual ~RenderWorkTask();
    void execute(GLContext* context) override;

private:
    std::vector<CommandBuffer*> m_cmds;
    std::vector<Semaphore*> m_waits;
    std::vector<Semaphore*> m_signals;
    Fence* m_fence = nullptr;
};

class PresentWorkTask : public WorkTask {
public:
    PresentWorkTask(GL3Queue* queue, SwapChain* swapChain, uint32_t imageIndex,
                    const std::vector<Semaphore*>& waits);
    virtual ~PresentWorkTask();
    void execute(GLContext* context) override;

private:
    SwapChain* m_swapChain{nullptr};
    std::vector<Semaphore*> m_waits;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE