#include "VulkanFence.h"

BEGIN_GFX_NAMESPACE

VulkanFence::VulkanFence(const vk::LogicDevice& device, bool createSignaled)
    : m_logicDevice(device), GfxObject(GfxObjectType::Fence) {
    VkFenceCreateInfo create_info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    if (createSignaled) {
        create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    VkResult result =
        vkCreateFence(m_logicDevice, &create_info, nullptr, &m_handle);

    CCASSERT(result == VK_SUCCESS, "create fence failed");
}

VulkanFence::~VulkanFence() {
    vkDestroyFence(m_logicDevice, m_handle, nullptr);
}

bool VulkanFence::wait(uint32_t timeout) {
    auto result = vkWaitForFences(m_logicDevice, 1, &m_handle, true, timeout);
    return result == VK_SUCCESS;
}

bool VulkanFence::reset() {
    auto result = vkResetFences(m_logicDevice, 1, &m_handle);
    return result == VK_SUCCESS;
}

bool VulkanFence::wait(
    const vk::LogicDevice& device, std::vector<VkFence> fences,
    uint32_t timeout = std::numeric_limits<uint32_t>::max()) {
    auto result =
        vkWaitForFences(device, fences.size(), fences.data(), true, timeout);
    return result == VK_SUCCESS;
}

bool VulkanFence::reset(const vk::LogicDevice& device,
                        std::vector<VkFence> fences) {
    if (fences.empty()) {
        return true;
    }

    auto result = vkResetFences(device, fences.size(), fences.data());
    return result == VK_SUCCESS;
}
END_GFX_NAMESPACE