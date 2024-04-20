#pragma once

#include "../base/Buffer.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Buffer : public Buffer {
public:
    GL3Buffer(GL3Device& device, BufferType type, uint32_t size);
    virtual ~GL3Buffer();
    uint32_t size() override { return m_size; }
    uint32_t capacity() override { return m_capacity; }

    void setSize(uint32_t size) override { m_size = size; }
    void update(const void* data, uint32_t size, uint32_t offset);

protected:
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }

private:
    GL3Device& m_device;
    uint32_t m_size{0};
    uint32_t m_capacity{0};
    GLenum m_target;
    GLenum m_glUsage;
    OGL_HANDLE m_handle{OGL_NULL_HANDLE};
    BufferType m_type;
};

END_GFX_NAMESPACE