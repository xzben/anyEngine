#include "VulkanQueue.h"

BEGIN_GFX_NAMESPACE
VulkanQueue::VulkanQueue(vk::LogicDevice& logicDeivce, QueueType queueType, float priority,
                         uint32_t familyIndex, uint32_t index)
    : m_logicDevice(logicDeivce),
      m_queueType(queueType),
      m_priority(priority),
      m_familyIndex(familyIndex),
      m_index(index) {
    vkGetDeviceQueue(m_logicDevice, m_familyIndex, m_index, &m_handle);
}

VulkanQueue::~VulkanQueue() {}
bool VulkanQueue::submit(const std::vector<CommandBuffer*>& cmd,
                         const std::vector<Semaphore*>& wait, const std::vector<Semaphore*>& signal,
                         Fence* fence) {
    return true;
}
uint32_t VulkanQueue::present(SwapChain* swapChain, uint32_t imageIndex,
                              const std::vector<Semaphore*>& waits) {
    return 0;
}
bool VulkanQueue::waitIdle() { return true; }

CommandPool* VulkanQueue::createPool(ResetMode resetModel) { return nullptr; }

END_GFX_NAMESPACE