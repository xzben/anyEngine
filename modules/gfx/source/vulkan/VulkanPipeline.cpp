#include "VulkanPipeline.h"

#include "VulkanRenderPass.h"
#include "VulkanShader.h"

BEGIN_GFX_NAMESPACE

std::vector<VkDynamicState> s_dynimacStates{
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
};

VulkanPipeline::VulkanPipeline(vk::LogicDevice& device,
                               const VulkanShader& shader,
                               const VulkanRenderPass& rendpass,
                               const PipelineState& state,
                               PrimitiveType primitiveType, uint32_t subpass)
    : m_logicDevice(device),
      m_shader(shader),
      m_rendpass(rendpass),
      m_subpass(subpass),
      m_state(state) {
    VkViewport viewport{};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = 0.0f;
    viewport.height   = 0.0f;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {0, 0};

    const auto& bindingDescription = shader.getVkVertexBindingDescriptions();
    const auto& attributeDescriptions =
        shader.getVkVertexAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount =
        static_cast<uint32_t>(bindingDescription.size());
    vertexInputInfo.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions   = bindingDescription.data();
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = vk::mapVkPrimitiveType(primitiveType);
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports    = &viewport;
    viewportState.scissorCount  = 1;
    viewportState.pScissors     = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    auto& rasterizerState = state.rasterize;
    rasterizer.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = rasterizerState.depthClampEnable;
    rasterizer.rasterizerDiscardEnable =
        rasterizerState.rasterizerDiscardEnable;
    rasterizer.polygonMode = vk::mapVkPolygonModel(rasterizerState.polygonMode);
    rasterizer.lineWidth   = rasterizerState.lineWidth;
    rasterizer.cullMode    = vk::mapVkCullModel(rasterizerState.cullMode);
    rasterizer.frontFace   = vk::mapVkFrontFace(rasterizerState.frontFace);
    rasterizer.depthBiasEnable =
        rasterizerState.depthBiasEnable ? VK_TRUE : VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable =
        state.multiSample.sampleShadingEnable ? VK_TRUE : VK_FALSE;
    multisampling.rasterizationSamples =
        vk::mapVkSampleCount(state.multiSample.rasterizationSamples);

    const auto& blendState = state.blend;
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable =
        blendState.blendEnable ? VK_TRUE : VK_FALSE;

    colorBlendAttachment.srcColorBlendFactor =
        vk::mapVkBlendFactor(blendState.srcColorFactor);
    colorBlendAttachment.dstColorBlendFactor =
        vk::mapVkBlendFactor(blendState.dstColorFactor);
    colorBlendAttachment.colorBlendOp =
        vk::mapVkBlendOp(blendState.colorBlendOp);
    colorBlendAttachment.srcAlphaBlendFactor =
        vk::mapVkBlendFactor(blendState.srcAlphaFactor);
    colorBlendAttachment.dstAlphaBlendFactor =
        vk::mapVkBlendFactor(blendState.dstAlphaFactor);
    colorBlendAttachment.alphaBlendOp =
        vk::mapVkBlendOp(blendState.alphaBlendOp);

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = blendState.logicOpEnable ? VK_TRUE : VK_FALSE;
    colorBlending.logicOp       = vk::mapVkBlendLogicOp(blendState.logicOp);
    colorBlending.attachmentCount   = 1;
    colorBlending.pAttachments      = &colorBlendAttachment;
    colorBlending.blendConstants[0] = blendState.constansts[0];
    colorBlending.blendConstants[1] = blendState.constansts[1];
    colorBlending.blendConstants[2] = blendState.constansts[2];
    colorBlending.blendConstants[3] = blendState.constansts[3];

    VkPipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    depthStencil.depthTestEnable =
        state.depthStencil.depthTestEnable ? VK_TRUE : VK_FALSE;
    depthStencil.depthWriteEnable =
        state.depthStencil.depthWriteEnable ? VK_TRUE : VK_FALSE;
    depthStencil.depthCompareOp =
        vk::mapVkCompareOp(state.depthStencil.depthCompareOp);
    depthStencil.depthBoundsTestEnable =
        state.depthStencil.depthBoundsTestEnable ? VK_TRUE : VK_FALSE;
    depthStencil.minDepthBounds    = state.depthStencil.minDepthBounds;
    depthStencil.maxDepthBounds    = state.depthStencil.maxDepthBounds;
    depthStencil.stencilTestEnable = state.depthStencil.stencilTestEnable;

    auto& front                    = state.depthStencil.front;
    depthStencil.front.failOp      = vk::mapVkStencilOp(front.failOp);
    depthStencil.front.passOp      = vk::mapVkStencilOp(front.passOp);
    depthStencil.front.depthFailOp = vk::mapVkStencilOp(front.depthFailOP);
    depthStencil.front.compareOp   = vk::mapVkCompareOp(front.compareOp);
    depthStencil.front.compareMask = front.compareMask;
    depthStencil.front.writeMask   = front.writeMask;
    depthStencil.front.reference   = front.reference;

    auto& back                    = state.depthStencil.back;
    depthStencil.back.failOp      = vk::mapVkStencilOp(back.failOp);
    depthStencil.back.passOp      = vk::mapVkStencilOp(back.passOp);
    depthStencil.back.depthFailOp = vk::mapVkStencilOp(back.depthFailOP);
    depthStencil.back.compareOp   = vk::mapVkCompareOp(back.compareOp);
    depthStencil.back.compareMask = back.compareMask;
    depthStencil.back.writeMask   = back.writeMask;
    depthStencil.back.reference   = back.reference;

    const auto& stageInfos     = shader.getStageInfos();
    const auto& pipelineLayout = shader.getPipelineLayout();

    VkPipelineDynamicStateCreateInfo dynamicInfo{
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
    dynamicInfo.dynamicStateCount = s_dynimacStates.size();
    dynamicInfo.pDynamicStates    = s_dynimacStates.data();

    VkGraphicsPipelineCreateInfo info{
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};

    info.stageCount          = stageInfos.size();
    info.pStages             = stageInfos.data();
    info.pVertexInputState   = &vertexInputInfo;
    info.pInputAssemblyState = &inputAssembly;
    info.pViewportState      = &viewportState;
    info.pRasterizationState = &rasterizer;
    info.pMultisampleState   = &multisampling;
    info.pColorBlendState    = &colorBlending;
    info.pDepthStencilState  = &depthStencil;
    info.layout              = pipelineLayout;
    info.renderPass          = m_rendpass;
    info.subpass             = m_subpass;
    info.pDynamicState       = &dynamicInfo;
    info.basePipelineHandle  = VK_NULL_HANDLE;
    auto result = vkCreateGraphicsPipelines(m_logicDevice, nullptr, 1, &info,
                                            nullptr, &m_handle);
    CCASSERT(result, "create pipeline failed");
}

VulkanPipeline::~VulkanPipeline() {
    if (m_handle) {
        vkDestroyPipeline(m_logicDevice, m_handle, nullptr);
    }
}

END_GFX_NAMESPACE