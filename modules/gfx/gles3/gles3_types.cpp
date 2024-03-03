#include "gles3_types.h"

BEGIN_GFX_NAMESPACE
inline GLenum map_gl_target(gfx::BufferUsage usage) {
    if (hasFlag(usage, BufferUsage::INDEX)) return GL_ELEMENT_ARRAY_BUFFER;

    if (hasFlag(usage, BufferUsage::VERTEX)) return GL_ARRAY_BUFFER;

    if (hasFlag(usage, BufferUsage::UNIFORM)) return GL_UNIFORM_BUFFER;

    if (hasFlag(usage, BufferUsage::STORAGE)) return GL_SHADER_STORAGE_BUFFER;

    return GL_NONE;
}

inline GLenum map_gl_buffer_usage(gfx::BufferUsage usage) {
    if (hasFlag(usage, BufferUsage::INDEX)) return GL_STATIC_DRAW;

    if (hasFlag(usage, BufferUsage::VERTEX)) return GL_STATIC_DRAW;

    if (hasFlag(usage, BufferUsage::UNIFORM)) return GL_DYNAMIC_DRAW;

    if (hasFlag(usage, BufferUsage::STORAGE)) return GL_DYNAMIC_DRAW;

    return GL_DYNAMIC_DRAW;
}

END_GFX_NAMESPACE