#pragma once

#include "../base/Event.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanEvent : public Event, public GfxObject {
public:
    VulkanEvent(const vk::LogicDevice& device, bool createDeviceOnly = false);
    virtual ~VulkanEvent();

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkEvent() const { return m_handle; }

    bool isSet();
    bool set();
    bool reset();

private:
    const vk::LogicDevice& m_logicDevice;
    VkEvent m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE