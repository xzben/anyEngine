#pragma once

#include "DescriptorSetLayout.h"
#include "LogicDevice.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class DescriptorSet {
public:
    DescriptorSet(const LogicDevice& device, const DescriptorSetLayout& layout,
                  VkDescriptorSet handle);

    DescriptorSet(DescriptorSet&& rhs)            = default;
    DescriptorSet& operator=(DescriptorSet&& rhs) = default;

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkDescriptorSet() const { return m_handle; }

private:
    const LogicDevice* m_logicDevice;
    VkDescriptorSet m_handle{VK_NULL_HANDLE};
};

END_VK_NAMESPACE
END_GFX_NAMESPACE