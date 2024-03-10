#include "VulkanCommandPool.h"

#include "VulkanCommandBuffer.h"

BEGIN_GFX_NAMESPACE
VulkanCommandPool::VulkanCommandPool(vk::LogicDevice& logicDevice,
                                     uint32_t queueFamilyIndex,
                                     ResetMode resetMode)
    : GfxObject(GfxObjectType::CommandPool),
      m_logicDevice(logicDevice),
      m_queueFamilyIndex(queueFamilyIndex),
      m_resetModel(resetMode) {
    VkCommandPoolCreateFlags flags;
    switch (resetMode) {
        case ResetMode::ResetIndividually:
        case ResetMode::AlwaysAllocate:
            flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            break;
        case ResetMode::ResetPool:
        default:
            flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            break;
    }

    VkCommandPoolCreateInfo create_info{
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    create_info.queueFamilyIndex = queueFamilyIndex;
    create_info.flags            = flags;

    if (vkCreateCommandPool(m_logicDevice, &create_info, nullptr, &m_handle)
        != VK_SUCCESS) {
        m_handle = VK_NULL_HANDLE;
    }
}
VulkanCommandPool::~VulkanCommandPool() {
    vkDestroyCommandPool(m_logicDevice, m_handle, nullptr);
}

VulkanCommandBuffer* VulkanCommandPool::alloc(CommandBufferLevel level) {
    auto& buffers = level == CommandBufferLevel::PRIMARY ? m_primaryBuffers
                                                         : m_secondaryBuffers;

    auto& count = level == CommandBufferLevel::PRIMARY ? m_activePrimaryCount
                                                       : m_activeSecondaryCount;

    if (count >= buffers.size()) {
        buffers.emplace_back(new VulkanCommandBuffer(*this, level));
    } else if (!buffers[count]) {
        buffers[count] = new VulkanCommandBuffer(*this, level);
    }

    auto ret = buffers[count];
    count++;
    return ret;
}

void VulkanCommandPool::reset() {
    
}

END_GFX_NAMESPACE