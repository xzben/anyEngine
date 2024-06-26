#pragma once

#include "../gl3_core_common.h"

BEGIN_GFX_NAMESPACE
class GL3CommandBuffer;
class GL3Semaphore;
class GL3Fence;
class GL3SwapChain;
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
    virtual ~WorkTask() = default;

    virtual void execute(GLContext* context) = 0;

    void finish();
    void waitFinish() { while (!m_finish.load()); }

protected:
    GL3Queue* m_queue{nullptr};
    std::atomic<bool> m_finish{false};
};

class RenderWorkTask : public WorkTask {
public:
    RenderWorkTask(GL3Queue* queue, const std::vector<CommandBuffer*>& cmd,
                   const std::vector<Semaphore*>& wait, const std::vector<Semaphore*>& signal,
                   Fence* fence);
    virtual ~RenderWorkTask();
    void execute(GLContext* context) override;

private:
    std::vector<GL3CommandBuffer*> m_cmds;
    std::vector<GL3Semaphore*> m_waits;
    std::vector<GL3Semaphore*> m_signals;
    GL3Fence* m_fence = nullptr;
};

class PresentWorkTask : public WorkTask {
public:
    PresentWorkTask(GL3Queue* queue, SwapChain* swapChain, uint32_t imageIndex,
                    const std::vector<Semaphore*>& waits);
    virtual ~PresentWorkTask();
    void execute(GLContext* context) override;

private:
    GL3SwapChain* m_swapChain{nullptr};
    std::vector<GL3Semaphore*> m_waits;
};

class CustomWorkTask : public WorkTask {
public:
    using WorkFunc = std::function<void(GLContext* ctx)>;

public:
    CustomWorkTask(GL3Queue* queue, WorkFunc func) : WorkTask(queue), m_func(func) {}

    void execute(GLContext* context) override;

private:
    WorkFunc m_func;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE