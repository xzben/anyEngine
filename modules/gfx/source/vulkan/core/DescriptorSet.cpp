#include "DescriptorSet.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE
DescriptorSet::DescriptorSet(const LogicDevice& device, const DescriptorSetLayout& layout,
                             VkDescriptorSet handle)
    : m_logicDevice(&device) {}

END_VK_NAMESPACE
END_GFX_NAMESPACE