#pragma once

#include "../base/CommandPool.h"
#include "VulkanCommandBuffer.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanCommandPool : public CommandPool, public GfxObject {
public:
    VulkanCommandPool(vk::LogicDevice& logicDevice, uint32_t queueFamilyIndex,
                      ResetMode resetMode);
    virtual ~VulkanCommandPool();

    virtual VulkanCommandBuffer* alloc(CommandBufferLevel level) override;
    virtual void reset() override;

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkCommandPool() const { return m_handle; }

    vk::LogicDevice& getLogicDevice() { return m_logicDevice; }

private:
    vk::LogicDevice& m_logicDevice;
    VkCommandPool m_handle{VK_NULL_HANDLE};
    uint32_t m_queueFamilyIndex{0};
    ResetMode m_resetModel{ResetMode::ResetPool};

    std::vector<VulkanCommandBuffer*> m_primaryBuffers;
    uint32_t m_activePrimaryCount{0};
    std::vector<VulkanCommandBuffer*> m_secondaryBuffers;
    uint32_t m_activeSecondaryCount{0};
};
END_GFX_NAMESPACE