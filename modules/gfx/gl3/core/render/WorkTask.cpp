#include "WorkTask.h"

#include "../../GL3CommandBuffer.h"
#include "../../GL3Fence.h"
#include "../../GL3SwapChain.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

//-----------------RenderWorkTask-----------------
void RenderWorkTask::execute(GLContext* context) {
    for (auto& item : m_waits) {
        item->wait();
    }
    //---------------------------------

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