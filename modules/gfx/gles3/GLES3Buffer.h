#pragma once

#include "../base/Buffer.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Buffer : public Buffer, public GfxObject {
public:
    GLES3Buffer(BufferUsage& usage, uint32_t size);
    virtual ~GLES3Buffer();
    virtual void update(const void* data, uint32_t size, uint32_t offset = 0);

private:
    GLenum m_target;
    GLenum m_glUsage;
    OGL_HANDLE m_handle{OGL_NULL_HANDLE};
};
END_GFX_NAMESPACE