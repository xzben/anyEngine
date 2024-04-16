#include "GL3CommandPool.h"

BEGIN_GFX_NAMESPACE
GL3CommandPool::GL3CommandPool(GL3Device& device) : m_device(device) {}
GL3CommandPool::~GL3CommandPool() {}
CommandBuffer* GL3CommandPool::alloc(CommandBufferLevel level) {
    return nullptr;
}
void GL3CommandPool::reset() {}
END_GFX_NAMESPACE