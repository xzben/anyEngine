#pragma once

#include "base/Queue.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanQueue : public Queue {
public:
    VulkanQueue(vk::LogicDevice& logicDeivce, QueueType queueType,
                float priority, uint32_t familyIndex, uint32_t index);

    virtual ~VulkanQueue();
    virtual void destroyPool(CommandPool* pool) override;

    virtual bool submit(const std::vector<CommandBuffer*>& cmd,
                        const std::vector<Semaphore*>& wait,
                        const std::vector<Semaphore*>& signal,
                        Fence* fence = nullptr) override;
    virtual uint32_t present(SwapChain* swapChain, uint32_t imageIndex,
                             const std::vector<Semaphore*>& waits) override;
    virtual bool waitIdle() override;

    virtual CommandPool* createPool(
        ResetMode resetModel = ResetMode::ResetPool) override;

    virtual QueueType getType() override { return m_queueType; }

    uint32_t familyIndex() { return m_familyIndex; }

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkQueue() const { return m_handle; }

private:
    vk::LogicDevice& m_logicDevice;
    QueueType m_queueType;
    float m_priority;
    uint32_t m_familyIndex{0};
    uint32_t m_index{0};
    VkQueue m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE