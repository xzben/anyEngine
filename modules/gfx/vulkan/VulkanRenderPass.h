#pragma once

#include "../base/RenderPass.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanRenderPass : public RenderPass {
public:
    VulkanRenderPass(const vk::LogicDevice& device, VkRenderPass handle,
                     const std::vector<Attachment>& attachments)
        : m_logicDevice(device), m_handle(handle), m_attachments(attachments) {}

public:
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkRenderPass() const { return m_handle; }

private:
    const vk::LogicDevice& m_logicDevice;
    VkRenderPass m_handle{VK_NULL_HANDLE};
    std::vector<Attachment> m_attachments;
};

END_GFX_NAMESPACE