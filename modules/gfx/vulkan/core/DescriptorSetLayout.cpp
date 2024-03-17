#include "DescriptorSetLayout.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

DescriptorSetLayout::DescriptorSetLayout(
    const LogicDevice& device,
    std::vector<VkDescriptorSetLayoutBinding> bindings)
    : m_bindings(std::move(bindings)) {
    VkDescriptorSetLayoutCreateInfo info{
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    info.bindingCount = m_bindings.size();
    info.pBindings    = m_bindings.data();

    auto result =
        vkCreateDescriptorSetLayout(device, &info, nullptr, &m_handle);
    assert(result == VK_SUCCESS);
}

DescriptorSetLayout::~DescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(m_logicDevice, m_handle, nullptr);
}
END_VK_NAMESPACE
END_GFX_NAMESPACE