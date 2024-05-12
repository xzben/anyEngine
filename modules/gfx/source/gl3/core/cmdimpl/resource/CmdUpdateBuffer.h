#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdUpdateBuffer : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::UPDATE_BUFFER;

public:
    CmdUpdateBuffer(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdUpdateBuffer() {}

    void init(Buffer* buffer, const void* pData, uint32_t size, uint32_t offset,
              bool staticData) {
        m_buffer = buffer;
        m_buffer->addRef();

        if (staticData) {
            m_pSrcData = pData;
        } else {
            m_data.resize(size);
            memcpy(m_data.data(), pData, size);
            m_pSrcData = nullptr;
        }
        m_size   = size;
        m_offset = offset;
    }

    virtual void reset() override {
        m_buffer->delRef();
        m_buffer   = nullptr;
        m_pSrcData = nullptr;
        m_size     = 0;
        m_data.swap(std::vector<uint8_t>());
    }

    virtual void execute(gl3::GLContext* context) override {
        OGL_HANDLE handle = m_buffer->getHandle<OGL_HANDLE>();
        const void* data  = m_pSrcData;
        if (data == nullptr) {
            data = m_data.data();
        }

        GL_CHECK(glBindBuffer(GL_COPY_WRITE_BUFFER, handle));
        void* dst{nullptr};
        GL_CHECK(dst = glMapBufferRange(
                     GL_COPY_WRITE_BUFFER, m_offset, m_size,
                     GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        if (nullptr == dst) {
            GL_CHECK(
                glBufferSubData(GL_COPY_WRITE_BUFFER, m_offset, m_size, data));
            return;
        } else {
            memcpy(dst, data, m_size);
        }
        GL_CHECK(glUnmapBuffer(GL_COPY_WRITE_BUFFER));
        GL_CHECK(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
    }

private:
    Buffer* m_buffer{nullptr};
    const void* m_pSrcData{nullptr};
    uint32_t m_size;
    std::vector<uint8_t> m_data;
    uint32_t m_offset;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE