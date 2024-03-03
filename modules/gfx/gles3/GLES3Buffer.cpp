#include "GLES3Buffer.h"

BEGIN_GFX_NAMESPACE

GLES3Buffer::GLES3Buffer(BufferUsage& usage, uint32_t size)
    : Buffer(usage, size), GfxObject(GfxObjectType::Buffer) {
    m_target  = map_gl_target(usage);
    m_glUsage = map_gl_buffer_usage(usage);
    GL_CHECK(glGenBuffers(1, &m_handle));
    GL_CHECK(glBindBuffer(m_target, m_handle));
    GL_CHECK(glBufferData(m_target, size, nullptr, m_glUsage));
    GL_CHECK(glBindBuffer(m_target, 0));
}

GLES3Buffer::~GLES3Buffer() {
    if (m_handle != OGL_NULL_HANDLE) {
        GL_CHECK(glDeleteBuffers(1, &m_handle));
        m_handle = OGL_NULL_HANDLE;
    }
}

void GLES3Buffer::update(const void* data, uint32_t size, uint32_t offset) {
    uint32_t curSize = size + offset;
    CCASSERT(curSize <= m_capacity, "update range need in capacity");
    m_size = m_size > curSize ? m_size : curSize;

    GL_CHECK(glBindBuffer(m_target, m_handle));
    void* dst{nullptr};
    GL_CHECK(dst = glMapBufferRange(
                 m_target, offset, size,
                 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
    if (!dst) {
        GL_CHECK(glBufferSubData(m_target, offset, size, data));
        return;
    }
    memcpy(dst, data, size);
    GL_CHECK(glUnmapBuffer(m_target));
    GL_CHECK(glBindBuffer(m_target, 0));
}

END_GFX_NAMESPACE