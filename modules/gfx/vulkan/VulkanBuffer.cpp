#include "VulkanBuffer.h"
BEGIN_GFX_NAMESPACE

VulkanBuffer::VulkanBuffer(const LogicDevice& logicDevice, BufferUsage& usage,
                           uint32_t size)
    : Buffer(usage, size),
      GfxObject(GfxObjectType::Buffer),
      m_logicDevice(logicDevice) {}

VulkanBuffer::~VulkanBuffer() {}

void VulkanBuffer::update(const void* data, uint32_t size, uint32_t offset) {}

END_GFX_NAMESPACE