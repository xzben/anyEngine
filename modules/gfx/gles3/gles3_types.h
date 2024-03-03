#pragma once

#include <glad/glad.h>

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE
extern inline GLenum map_gl_target(gfx::BufferUsage usage);
extern inline GLenum map_gl_buffer_usage(gfx::BufferUsage usage);
END_GFX_NAMESPACE