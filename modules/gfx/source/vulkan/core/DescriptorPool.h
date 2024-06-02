#pragma once

#include "DescriptorSetLayout.h"
#include "LogicDevice.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class DescriptorPool {
private:
    static constexpr uint32_t MAX_SETS_PER_POOL = 16;

public:
    DescriptorPool(const LogicDevice& device, const DescriptorSetLayout& setlayout,
                   uint32_t poolSize = MAX_SETS_PER_POOL);
    DescriptorPool(DescriptorPool&& rhs)            = default;
    DescriptorPool& operator=(DescriptorPool&& rhs) = default;
    virtual ~DescriptorPool();

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkDescriptorPool() const { return m_handle; }

private:
    const LogicDevice* m_logicDevice;
    VkDescriptorPool m_handle{VK_NULL_HANDLE};
};
END_VK_NAMESPACE
END_GFX_NAMESPACE