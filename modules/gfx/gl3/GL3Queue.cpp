#include "GL3Queue.h"

#include "GL3CommandPool.h"

BEGIN_GFX_NAMESPACE
GL3Queue::GL3Queue(GL3Device& device, QueueType queueType, float priority)
    : m_device(device), m_queueType(queueType), m_priority(priority) {}
GL3Queue::~GL3Queue() {}

void GL3Queue::destroyPool(CommandPool* pool) { pool->delRef(); }
CommandPool* GL3Queue::createPool(ResetMode resetModel) {
    return new GL3CommandPool(m_device, *this, resetModel);
}

bool GL3Queue::submit(const std::vector<CommandBuffer*>& cmd,
                      const std::vector<Semaphore*>& wait,
                      const std::vector<Semaphore*>& signal, Fence* fence) {
    return true;
}

uint32_t GL3Queue::present(SwapChain* swapChain, uint32_t imageIndex,
                           const std::vector<Semaphore*>& waits) {
    return 0;
}
bool GL3Queue::waitIdle() { return true; }

END_GFX_NAMESPACE