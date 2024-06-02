#include "DescriptorPool.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

DescriptorPool::DescriptorPool(const LogicDevice& device, const DescriptorSetLayout& setlayout,
                               uint32_t poolSize)
    : m_logicDevice(&device) {}
END_VK_NAMESPACE
END_GFX_NAMESPACE