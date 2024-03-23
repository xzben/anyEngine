#pragma once

#include "../base/RenderPass.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanRenderSubpass : public RenderSubpass {
public:
    friend class VulkanRenderPassBuilder;
    VulkanRenderSubpass(const vk::LogicDevice& device);

    virtual void addInput(uint32_t attachment, ImageLayout layout) override;
    virtual void addColor(uint32_t attachment, ImageLayout layout) override;
    virtual void addResolve(uint32_t attachment, ImageLayout layout) override;
    virtual void addDepthStencil(uint32_t attachment,
                                 ImageLayout layout) override;
    virtual void addPerserve(uint32_t attachment) override;

private:
    const vk::LogicDevice& m_logicDevice;
    std::vector<VkAttachmentReference> m_inputAttachments;
    std::vector<VkAttachmentReference> m_colorAttachments;
    std::vector<VkAttachmentReference> m_resolveAttachments;
    std::vector<VkAttachmentReference> m_depthStencilAttachment;
    std::vector<uint32_t> m_preserveAttachments;
};

class VulkanRenderPass : public RenderPass, public GfxObject {
public:
    VulkanRenderPass(const vk::LogicDevice& device, VkRenderPass handle,
                     const std::vector<Attachment>& attachments)
        : GfxObject(GfxObjectType::RenderPass),
          m_logicDevice(device),
          m_handle(handle),
          m_attachments(attachments) {}

public:
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkRenderPass() const { return m_handle; }

private:
    const vk::LogicDevice& m_logicDevice;
    VkRenderPass m_handle{VK_NULL_HANDLE};
    std::vector<Attachment> m_attachments;
};

class VulkanRenderPassBuilder : public RenderPassBuilder {
public:
    VulkanRenderPassBuilder(const vk::LogicDevice& device,
                            const std::vector<Attachment>& attachments);
    virtual VulkanRenderSubpass* addSubpass() override;
    virtual VulkanRenderPass* build() override;

private:
    std::vector<VulkanRenderSubpass*> m_subpasses;
    const vk::LogicDevice& m_device;
};
END_GFX_NAMESPACE