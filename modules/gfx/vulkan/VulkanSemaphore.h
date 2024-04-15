#pragma once

#include "../base/Semaphore.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanSemaphore : public Semaphore {
public:
    VulkanSemaphore(const vk::LogicDevice& device);
    virtual ~VulkanSemaphore();

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkSemaphore() const { return m_handle; }
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }

private:
    const vk::LogicDevice& m_logicDevice;
    VkSemaphore m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE