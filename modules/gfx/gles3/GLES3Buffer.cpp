#include "GLES3Buffer.h"

BEGIN_GFX_NAMESPACE

static GLenum g_buffer_tages[] = {
    GL_ELEMENT_ARRAY_BUFFER,   // index
    GL_ARRAY_BUFFER,           // vertex
    GL_UNIFORM_BUFFER,         // uniform
    GL_SHADER_STORAGE_BUFFER,  // storage
    0,                         // stage
};

static GLenum g_buffer_usage[] = {
    GL_STATIC_DRAW,   // index
    GL_STATIC_DRAW,   // vertex
    GL_DYNAMIC_DRAW,  // uniform
    GL_DYNAMIC_DRAW,  // storage
    0,                // stage
};

GLES3Buffer::GLES3Buffer(gles::GLContext& context, BufferType type,
                         uint32_t size)
    : GfxObject(GfxObjectType::Buffer),
      m_context(context),
      m_type(type),
      m_capacity(size),
      m_size(0) {
    m_target  = g_buffer_tages[int(type)];
    m_glUsage = g_buffer_usage[int(type)];

    if (m_type != BufferType::STAGE) {
        GL_CHECK(glGenBuffers(1, &m_handle));
        GL_CHECK(glBindBuffer(m_target, m_handle));
        GL_CHECK(glBufferData(m_target, size, nullptr, m_glUsage));
        GL_CHECK(glBindBuffer(m_target, 0));
    } else {
        m_stageData.resize(size);
    }
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

    if (m_type != BufferType::STAGE) {
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
    } else {
        memcpy((uint8_t*)m_stageData.data() + offset, data, size);
    }
}

END_GFX_NAMESPACE