#pragma once

#include "../gl3_core_common.h"

BEGIN_GFX_NAMESPACE
class GL3CommandBuffer;
class Semaphore;
class Fence;
class SwapChain;

BEGIN_GL3_CORE_NAMESPACE

class GLContext;

class WorkTask {
public:
    virtual void execute(GLContext* context) = 0;
};

class RenderWorkTask : public WorkTask {
public:
    void execute(GLContext* context) override;

private:
    std::vector<Semaphore*> m_waits;
    std::vector<Semaphore*> m_signals;
    Fence* m_fence = nullptr;
};

class PresentWorkTask : public WorkTask {
public:
    void execute(GLContext* context) override;

private:
    SwapChain* m_swapChain{nullptr};
    std::vector<Semaphore*> m_waits;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE