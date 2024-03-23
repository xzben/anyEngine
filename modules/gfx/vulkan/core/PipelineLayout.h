#pragma once

#include "DescriptorSetLayout.h"
#include "LogicDevice.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class PipelineLayout {
public:
    PipelineLayout(const LogicDevice& device,
                   const std::vector<DescriptorSetLayout>& descriptSetLayouts,
                   const std::vector<VkPushConstantRange>& pushConstants = {});
    virtual ~PipelineLayout();

    PipelineLayout(const PipelineLayout& rhs)            = delete;
    PipelineLayout& operator=(const PipelineLayout& rhs) = delete;

    PipelineLayout(PipelineLayout&& rhs);
    PipelineLayout& operator=(PipelineLayout&& rhs);

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkPipelineLayout() const { return m_handle; }

private:
    void initVkDescriptorSetLayouts();

private:
    const LogicDevice* m_logicDevice{nullptr};
    VkPipelineLayout m_handle{VK_NULL_HANDLE};
    std::vector<DescriptorSetLayout> m_descriptSetLayouts;
    std::vector<VkPushConstantRange> m_pushConstants;
    std::vector<VkDescriptorSetLayout> m_vkSetLayouts;
};

END_VK_NAMESPACE
END_GFX_NAMESPACE