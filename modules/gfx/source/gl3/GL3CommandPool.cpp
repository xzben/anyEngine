#include "GL3CommandPool.h"

#include "GL3CommandBuffer.h"

BEGIN_GFX_NAMESPACE
GL3CommandPool::GL3CommandPool(GL3Device& device, GL3Queue& queue, ResetMode resetModel)
    : m_device(device), m_queue(queue), m_resetModel(resetModel) {}
GL3CommandPool::~GL3CommandPool() { m_buffers.clear(); }
GL3CommandBuffer* GL3CommandPool::alloc(CommandBufferLevel level) {
    GL3CommandBuffer* cmd = nullptr;

    uint32_t count = m_buffers.size();
    if (m_resetModel == ResetMode::ResetPool) {
        uint32_t index = m_activeCount;
        if (index >= count) {
            m_buffers.emplace_back(new GL3CommandBuffer(m_device, *this, index));
        }
        m_activeCount++;
        cmd = m_buffers[index];
    } else {
        cmd = new GL3CommandBuffer(m_device, *this, count);
    }

    return cmd;
}

void GL3CommandPool::free(CommandBuffer* buffer) {
    if (m_resetModel == ResetMode::ResetPool) {
    } else {
        buffer->delRef();
    }
}

void GL3CommandPool::reset() {
    if (m_resetModel == ResetMode::ResetPool) {
        for (auto& cmd : m_buffers) {
            cmd->reset();
        }
        m_activeCount = 0;
    }
}

END_GFX_NAMESPACE