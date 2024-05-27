#include "GL3Queue.h"

#include "GL3CommandPool.h"
#include "GL3Device.h"
#include "core/render/WorkTask.h"

BEGIN_GFX_NAMESPACE
GL3Queue::GL3Queue(GL3Device& device, QueueType queueType, float priority)
    : m_device(device), m_queueType(queueType), m_priority(priority) {}
GL3Queue::~GL3Queue() {}

void GL3Queue::destroyPool(CommandPool* pool) { pool->delRef(); }

CommandPool* GL3Queue::createPool(ResetMode resetModel) {
    return new GL3CommandPool(m_device, *this, resetModel);
}

bool GL3Queue::submit(const std::vector<CommandBuffer*>& cmd, const std::vector<Semaphore*>& wait,
                      const std::vector<Semaphore*>& signal, Fence* fence) {
    gl3::WorkTask* task = new gl3::RenderWorkTask(this, cmd, wait, signal, fence);
    addTask(task);
    return true;
}

uint32_t GL3Queue::present(SwapChain* swapChain, uint32_t imageIndex,
                           const std::vector<Semaphore*>& waits) {
    gl3::WorkTask* task = new gl3::PresentWorkTask(this, swapChain, imageIndex, waits);

    addTask(task);
    return 0;
}

void GL3Queue::addTask(gl3::WorkTask* task) {
    m_workCount++;
    m_device.addTask(task);
}

void GL3Queue::handleTaskFinish(gl3::WorkTask* task) {
    std::lock_guard<std::mutex> locker(m_idleConditionlock);
    if (--m_workCount <= 0) {
        m_idleCondition.notify_all();
    }
}

bool GL3Queue::waitIdle() {
    if (m_workCount.load() <= 0) return true;

    std::unique_lock<std::mutex> locker(m_idleConditionlock);
    m_idleCondition.wait(locker, [&]() { return m_workCount <= 0; });

    return true;
}

END_GFX_NAMESPACE