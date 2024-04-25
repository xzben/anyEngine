#include "VulkanFence.h"

BEGIN_GFX_NAMESPACE

VulkanFence::VulkanFence(const vk::LogicDevice& device, bool createSignaled)
    : m_logicDevice(device) {
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

WaitResult VulkanFence::wait(uint64_t timeout) {
    if (timeout == 0) {
        timeout = std::numeric_limits<uint64_t>::max();
    }
    auto result = vkWaitForFences(m_logicDevice, 1, &m_handle, true, timeout);
    return result == VK_SUCCESS ? WaitResult::SUCCESS : WaitResult::TIMEOUT;
}

bool VulkanFence::reset() {
    auto result = vkResetFences(m_logicDevice, 1, &m_handle);
    return result == VK_SUCCESS;
}

WaitResult VulkanFence::wait(const vk::LogicDevice& device,
                             std::vector<VkFence> fences, uint64_t timeout) {
    if (timeout == 0) {
        timeout = std::numeric_limits<uint64_t>::max();
    }

    auto result =
        vkWaitForFences(device, fences.size(), fences.data(), true, timeout);
    return result == VK_SUCCESS ? WaitResult::SUCCESS : WaitResult::TIMEOUT;
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