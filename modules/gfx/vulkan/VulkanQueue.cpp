#include "VulkanQueue.h"

BEGIN_GFX_NAMESPACE
VulkanQueue::VulkanQueue(vk::LogicDevice& logicDeivce, uint32_t familyIndex,
                         uint32_t index)
    : GfxObject(GfxObjectType::Queue),
      m_logicDevice(logicDeivce),
      m_familyIndex(familyIndex),
      m_index(index) {
    vkGetDeviceQueue(m_logicDevice, m_familyIndex, m_index, &m_handle);
}

END_GFX_NAMESPACE