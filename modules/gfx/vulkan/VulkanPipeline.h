#pragma once

#include "../base/Pipeline.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"

BEGIN_GFX_NAMESPACE
class VulkanShader;
class VulkanRenderPass;

class VulkanPipeline : public Pipeline, public GfxObject {
public:
    VulkanPipeline(vk::LogicDevice& device, const VulkanShader& shader,
                   const VulkanRenderPass& rendpass, const PipelineState& state,
                   PrimitiveType primitiveType, uint32_t subpass);
    virtual ~VulkanPipeline();

public:
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkPipeline() const { return m_handle; }

private:
    PrimitiveType m_primitiveType{PrimitiveType::TRIANGLE_LIST};
    const vk::LogicDevice& m_logicDevice;
    const VulkanShader& m_shader;
    const VulkanRenderPass& m_rendpass;
    uint32_t m_subpass;
    const PipelineState m_state;
    VkPipeline m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE