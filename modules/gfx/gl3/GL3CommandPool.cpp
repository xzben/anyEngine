#include "GL3CommandPool.h"

#include "GL3CommandBuffer.h"

BEGIN_GFX_NAMESPACE
GL3CommandPool::GL3CommandPool(GL3Device& device, GL3Queue& queue)
    : m_device(device), m_queue(queue) {}
GL3CommandPool::~GL3CommandPool() {}
CommandBuffer* GL3CommandPool::alloc(CommandBufferLevel level) {
    return nullptr;
}

void GL3CommandPool::free(CommandBuffer* buffer) { buffer->delRef(); }
void GL3CommandPool::reset() {}
END_GFX_NAMESPACE