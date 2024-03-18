#pragma once

#include "../base/Buffer.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Buffer : public Buffer, public GfxObject {
public:
    GLES3Buffer(gles::GLContext& context, BufferType type, uint32_t size);
    virtual ~GLES3Buffer();
    virtual void update(const void* data, uint32_t size, uint32_t offset = 0);
    virtual uint32_t size() { return m_size; }
    virtual uint32_t capacity() { return m_capacity; }

protected:
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }

private:
    gles::GLContext& m_context;
    uint32_t m_size{0};
    uint32_t m_capacity{0};
    GLenum m_target;
    GLenum m_glUsage;
    OGL_HANDLE m_handle{OGL_NULL_HANDLE};
    BufferType m_type;
    std::vector<uint8_t> m_stageData;
};

END_GFX_NAMESPACE