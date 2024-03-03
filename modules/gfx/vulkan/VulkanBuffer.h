#pragma once

#include "../base/Buffer.h"
#include "core/core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class LogicDevice;

class VulkanBuffer : public Buffer, public GfxObject {
public:
    VulkanBuffer(const LogicDevice& logicDevice, BufferUsage& usage,
                 uint32_t size);
    virtual ~VulkanBuffer();
    virtual void update(const void* data, uint32_t size, uint32_t offset = 0);

private:
    const LogicDevice& m_logicDevice;
    VkBuffer m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE