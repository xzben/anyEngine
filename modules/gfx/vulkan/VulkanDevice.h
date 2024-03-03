#pragma once

#include "../base/Device.h"
#include "vulkan_common.h"

BEGIN_GFX_NAMESPACE

class VulkanDevice : public Device {
public:
private:
    VmaAllocator m_allocator = VK_NULL_HANDLE;
};

END_GFX_NAMESPACE