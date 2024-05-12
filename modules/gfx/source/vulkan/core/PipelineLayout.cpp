#include "PipelineLayout.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

PipelineLayout::PipelineLayout(
    const LogicDevice& device,
    const std::vector<DescriptorSetLayout>& descriptSetLayouts,
    const std::vector<VkPushConstantRange>& pushConstants)
    : m_logicDevice(&device),
      m_descriptSetLayouts(descriptSetLayouts),
      m_pushConstants(pushConstants) {
    initVkDescriptorSetLayouts();
    VkPipelineLayoutCreateInfo create_info{
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    create_info.setLayoutCount = m_vkSetLayouts.size();
    create_info.pSetLayouts    = m_vkSetLayouts.data();

    create_info.pushConstantRangeCount = pushConstants.size();
    create_info.pPushConstantRanges    = pushConstants.data();

    auto result = vkCreatePipelineLayout(*m_logicDevice, &create_info, nullptr,
                                         &m_handle);
    assert(result == VK_SUCCESS);
}

PipelineLayout::~PipelineLayout() {
    vkDestroyPipelineLayout(*m_logicDevice, m_handle, nullptr);
}

void PipelineLayout::initVkDescriptorSetLayouts() {
    int count = m_descriptSetLayouts.size();
    m_vkSetLayouts.resize(count);

    for (int i = 0; i < count; i++) {
        m_vkSetLayouts[i] = m_descriptSetLayouts[i];
    }
}
PipelineLayout::PipelineLayout(PipelineLayout&& rhs)
    : m_logicDevice(rhs.m_logicDevice), m_handle(rhs.m_handle) {
    m_descriptSetLayouts = (rhs.m_descriptSetLayouts);
    m_pushConstants      = (rhs.m_pushConstants);
    rhs.m_handle         = VK_NULL_HANDLE;
    rhs.m_logicDevice    = nullptr;
}

PipelineLayout& PipelineLayout::operator=(PipelineLayout&& rhs) {
    this->m_logicDevice        = rhs.m_logicDevice;
    this->m_pushConstants      = std::move(rhs.m_pushConstants);
    this->m_descriptSetLayouts = std::move(rhs.m_descriptSetLayouts);
    this->m_handle             = rhs.m_handle;

    rhs.m_handle      = VK_NULL_HANDLE;
    rhs.m_logicDevice = nullptr;
    return *this;
}

END_VK_NAMESPACE
END_GFX_NAMESPACE
