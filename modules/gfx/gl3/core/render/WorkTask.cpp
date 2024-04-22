#include "WorkTask.h"

#include "../../GL3CommandBuffer.h"
#include "../../GL3Fence.h"
#include "../../GL3Queue.h"
#include "../../GL3SwapChain.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

void WorkTask::finish() { m_queue->handleTaskFinish(this); }
//-----------------RenderWorkTask-----------------
RenderWorkTask::RenderWorkTask(GL3Queue* queue,
                               const std::vector<CommandBuffer*>& cmds,
                               const std::vector<Semaphore*>& waits,
                               const std::vector<Semaphore*>& signals,
                               Fence* fence)
    : WorkTask(queue),
      m_cmds(cmds),
      m_waits(waits),
      m_signals(signals),
      m_fence(fence) {
    for (auto& cmd : m_cmds) {
        cmd->addRef();
    }
    for (auto& wait : m_waits) {
        wait->addRef();
    }
    for (auto& singal : m_signals) {
        singal->addRef();
    }
    if (m_fence) m_fence->addRef();
}

RenderWorkTask::~RenderWorkTask() {
    for (auto& cmd : m_cmds) {
        cmd->delRef();
    }
    for (auto& wait : m_waits) {
        wait->delRef();
    }
    for (auto& singal : m_signals) {
        singal->delRef();
    }
    if (m_fence) m_fence->delRef();
}

void RenderWorkTask::execute(GLContext* context) {
    for (auto& item : m_waits) {
        item->wait();
    }
    //---------------------------------
    for (auto& cmd : m_cmds) {
        dynamic_cast<GL3CommandBuffer*>(cmd)->execute(context);
    }
    //-------------------------------
    for (auto& item : m_waits) {
        item->signal();
    }
    if (m_fence) {
        dynamic_cast<GL3Fence*>(m_fence)->signal();
    }
}
//-----------------PresentWorkTask-----------------
void PresentWorkTask::execute(GLContext* context) {
    for (auto& item : m_waits) {
        item->wait();
    }
    dynamic_cast<GL3SwapChain*>(m_swapChain)->swapBuffer();
}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE