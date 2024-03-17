#include "VulkanEvent.h"

BEGIN_GFX_NAMESPACE

VulkanEvent::VulkanEvent(const vk::LogicDevice& device, bool createDeviceOnly)
    : m_logicDevice(device), GfxObject(GfxObjectType::Event) {
    VkEventCreateInfo info{VK_STRUCTURE_TYPE_EVENT_CREATE_INFO};

    if (createDeviceOnly) {
        info.flags = VK_EVENT_CREATE_DEVICE_ONLY_BIT;
    }

    VkResult result = vkCreateEvent(device, &info, nullptr, &m_handle);
    CCASSERT(result == VK_SUCCESS, "create event failed");
}

VulkanEvent::~VulkanEvent() {
    vkDestroyEvent(m_logicDevice, m_handle, nullptr);
}

bool VulkanEvent::isSet() {
    auto result = vkGetEventStatus(m_logicDevice, m_handle);

    return result == VK_EVENT_SET;
}

bool VulkanEvent::set() {
    auto result = vkSetEvent(m_logicDevice, m_handle);

    return result == VK_SUCCESS;
}

bool VulkanEvent::reset() {
    auto result = vkResetEvent(m_logicDevice, m_handle);

    return result == VK_SUCCESS;
}

END_GFX_NAMESPACE