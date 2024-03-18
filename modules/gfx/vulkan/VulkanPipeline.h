#pragma once

#include "../base/Pipeline.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanPipeline : public Pipeline, public GfxObject {
public:
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkPipeline() const { return m_handle; }

private:
    const vk::LogicDevice& m_logicDevice;
    VkPipeline m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE