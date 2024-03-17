#pragma once

#include "DescriptorSetLayout.h"
#include "LogicDevice.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class PipelineLayout {
public:
    PipelineLayout(const LogicDevice& device,
                   const std::vector<VkDescriptorSetLayout>& descriptSetLayouts,
                   const std::vector<VkPushConstantRange>& pushConstants = {});
    virtual ~PipelineLayout();

    PipelineLayout(const PipelineLayout& rhs)            = delete;
    PipelineLayout& operator=(const PipelineLayout& rhs) = delete;

    PipelineLayout(const PipelineLayout&& rhs);
    PipelineLayout& operator=(const PipelineLayout&& rhs);

    PipelineLayout(const PipelineLayout&& rhs);
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkPipelineLayout() const { return m_handle; }

private:
    const LogicDevice* m_logicDevice{nullptr};
    VkPipelineLayout m_handle{VK_NULL_HANDLE};
    std::vector<VkDescriptorSetLayout> m_descriptSetLayouts;
    std::vector<VkPushConstantRange> m_pushConstants;
};

END_VK_NAMESPACE
END_GFX_NAMESPACE