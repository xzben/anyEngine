#include "VulkanSemaphore.h"

BEGIN_GFX_NAMESPACE

VulkanSemaphore::VulkanSemaphore(const vk::LogicDevice& device)
    : m_logicDevice(device) {
    VkSemaphoreCreateInfo create_info{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VkResult result =
        vkCreateSemaphore(m_logicDevice, &create_info, nullptr, &m_handle);
    assert(result == VK_SUCCESS);
}

VulkanSemaphore::~VulkanSemaphore() {
    vkDestroySemaphore(m_logicDevice, m_handle, nullptr);
}

END_GFX_NAMESPACE