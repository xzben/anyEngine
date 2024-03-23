#include "VulkanRenderPass.h"

BEGIN_GFX_NAMESPACE

VulkanRenderSubpass::VulkanRenderSubpass(const vk::LogicDevice& device)
    : m_logicDevice(device) {}

void VulkanRenderSubpass::addInput(uint32_t attachment, ImageLayout layout) {
    m_inputAttachments.push_back({attachment, vk::mapVkImageLayout(layout)});
}
void VulkanRenderSubpass::addColor(uint32_t attachment, ImageLayout layout) {
    m_colorAttachments.push_back({attachment, vk::mapVkImageLayout(layout)});
}
void VulkanRenderSubpass::addResolve(uint32_t attachment, ImageLayout layout) {
    m_resolveAttachments.push_back({attachment, vk::mapVkImageLayout(layout)});
}
void VulkanRenderSubpass::addDepthStencil(uint32_t attachment,
                                          ImageLayout layout) {
    m_depthStencilAttachment.push_back(
        {attachment, vk::mapVkImageLayout(layout)});
}
void VulkanRenderSubpass::addPerserve(uint32_t attachment) {
    m_preserveAttachments.push_back(attachment);
}

//--------------------------------------------------------------

VulkanRenderPassBuilder::VulkanRenderPassBuilder(
    const vk::LogicDevice& device, const std::vector<Attachment>& attachments)
    : RenderPassBuilder(attachments), m_device(device) {}

VulkanRenderSubpass* VulkanRenderPassBuilder::addSubpass() {
    auto obj = new VulkanRenderSubpass(m_device);
    m_subpasses.push_back(obj);

    return obj;
}
VulkanRenderPass* VulkanRenderPassBuilder::build() {
    std::vector<VkAttachmentDescription> attachments(m_attachments.size());
    for (auto i = 0; i < m_attachments.size(); ++i) {
        attachments[i].flags   = {};
        attachments[i].format  = vk::mapVkFormat(m_attachments[i].format);
        attachments[i].samples = vk::mapVkSampleCount(m_attachments[i].samples);
        attachments[i].loadOp  = vk::mapVkLoadOp(m_attachments[i].load_op);
        attachments[i].storeOp = vk::mapVkStoreOp(m_attachments[i].store_op);
        attachments[i].stencilLoadOp =
            vk::mapVkLoadOp(m_attachments[i].stencil_load_op);
        attachments[i].stencilStoreOp =
            vk::mapVkStoreOp(m_attachments[i].stencil_store_op);
        attachments[i].initialLayout =
            vk::mapVkImageLayout(m_attachments[i].initial_layout);
        attachments[i].finalLayout =
            vk::mapVkImageLayout(m_attachments[i].final_layout);
    }

    std::vector<VkSubpassDescription> subpasses(m_subpasses.size());
    for (auto i = 0; i < m_subpasses.size(); ++i) {
        subpasses[i].flags             = {};
        subpasses[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        subpasses[i].inputAttachmentCount =
            m_subpasses[i]->m_inputAttachments.size();

        subpasses[i].pInputAttachments =
            m_subpasses[i]->m_inputAttachments.data();

        subpasses[i].colorAttachmentCount =
            m_subpasses[i]->m_colorAttachments.size();
        subpasses[i].pColorAttachments =
            m_subpasses[i]->m_colorAttachments.data();

        subpasses[i].pResolveAttachments =
            m_subpasses[i]->m_resolveAttachments.data();

        subpasses[i].pDepthStencilAttachment =
            m_subpasses[i]->m_depthStencilAttachment.data();

        subpasses[i].preserveAttachmentCount =
            m_subpasses[i]->m_preserveAttachments.size();
        subpasses[i].pPreserveAttachments =
            m_subpasses[i]->m_preserveAttachments.data();
    }

    std::vector<VkSubpassDependency> dependency(m_dependencies.size());
    for (auto i = 0; i < m_dependencies.size(); ++i) {
        dependency[i].srcSubpass = m_dependencies[i].src.subpass;
        dependency[i].dstSubpass = m_dependencies[i].dst.subpass;
        dependency[i].srcStageMask =
            vk::mapVkPipelineStage(m_dependencies[i].src.stage_mask);
        dependency[i].dstStageMask =
            vk::mapVkPipelineStage(m_dependencies[i].dst.stage_mask);
        dependency[i].srcAccessMask =
            vk::mapVkAccessFlags(m_dependencies[i].src.access_flags);
        dependency[i].dstAccessMask =
            vk::mapVkAccessFlags(m_dependencies[i].dst.access_flags);
        dependency[i].dependencyFlags =
            vk::mapVkDependencyFlags(m_dependencies[i].flags);
    }

    VkRenderPassCreateInfo create_info{
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    create_info.attachmentCount = attachments.size();
    create_info.pAttachments    = attachments.data();
    create_info.subpassCount    = subpasses.size();
    create_info.pSubpasses      = subpasses.data();
    create_info.dependencyCount = dependency.size();
    create_info.pDependencies   = dependency.data();

    VkRenderPass handle = VK_NULL_HANDLE;
    if (vkCreateRenderPass(m_device, &create_info, nullptr, &handle)
        != VK_SUCCESS) {
        return nullptr;
    }

    return new VulkanRenderPass(m_device, handle, m_attachments);
}
END_GFX_NAMESPACE