#pragma once

#include "../base/Queue.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanQueue : public Queue, public GfxObject {
public:
    VulkanQueue(vk::LogicDevice& logicDeivce, uint32_t familyIndex,
                uint32_t index);

    virtual ~VulkanQueue();
    virtual bool submit(CommandBuffer* cmd, Fence* fence = nullptr) override;
    virtual uint32_t present(SwapChain* swapChain, uint32_t imageIndex,
                             const std::vector<Semaphore*>& waits) override;
    virtual bool waitIdle() override;

    uint32_t familyIndex() { return m_familyIndex; }

    operator bool() { return m_handle != VK_NULL_HANDLE; }
    operator VkQueue() { return m_handle; }

private:
    vk::LogicDevice& m_logicDevice;
    uint32_t m_familyIndex{0};
    uint32_t m_index{0};
    VkQueue m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE