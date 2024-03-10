#pragma once

#include <memory>

#include "../base/Device.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanDevice : public Device {
public:
private:
    VmaAllocator m_allocator = VK_NULL_HANDLE;
    std::unique_ptr<vk::LogicDevice> m_logicDevice;
};

END_GFX_NAMESPACE