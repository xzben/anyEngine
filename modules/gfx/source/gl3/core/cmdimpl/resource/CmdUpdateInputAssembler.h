#pragma once

#include "../../../GL3InputAssembler.h"
#include "../CmdBase.h"
BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdUpdateInputAssembler : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::UPDATE_INPUTASSEMBLER;

public:
    CmdUpdateInputAssembler(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdUpdateInputAssembler() {}

    void init(InputAssembler* input, const UpdateAssemblerInfo& info) {
        m_input = dynamic_cast<GL3InputAssembler*>(input);
        m_input->addRef();

        m_info = info;

        if (!m_info.staticData) {
            if (m_info.pVertexData) {
                m_vertexData.resize(m_info.vertexDataSize);
                memcpy(m_vertexData.data(), m_info.pVertexData, m_info.vertexDataSize);
            }

            if (m_info.pIndexData) {
                m_indexData.resize(m_info.indexDataSize);
                memcpy(m_indexData.data(), m_info.pIndexData, m_info.indexDataSize);
            }

            if (m_info.pInstanceData) {
                m_instanceData.resize(m_info.instanceDataSize);
                memcpy(m_instanceData.data(), m_info.pInstanceData, m_info.instanceDataSize);
            }
        }
    }

    virtual void reset() override {
        m_input->delRef();
        {
            std::vector<uint8_t> empty;
            m_vertexData.swap(empty);
        }

        {
            std::vector<uint8_t> empty;
            m_indexData.swap(empty);
        }

        {
            std::vector<uint8_t> empty;
            m_instanceData.swap(empty);
        }
    }

    virtual void execute(gl3::GLContext* context) override {
        const void* pVertexData   = m_info.pVertexData;
        const void* pIndexData    = m_info.pIndexData;
        const void* pInstanceData = m_info.pInstanceData;

        if (!m_info.staticData) {
            pVertexData   = m_vertexData.data();
            pIndexData    = m_indexData.data();
            pInstanceData = m_instanceData.data();
        }

        if (m_info.pVertexData) {
            updateBuffer(m_input->getVertexBuffer(), pVertexData, m_info.vertexDataSize, 0);
        }

        if (m_info.pIndexData) {
            updateBuffer(m_input->getIndexBuffer(), pIndexData, m_info.indexDataSize, 0);
        }

        if (m_info.pInstanceData) {
            updateBuffer(m_input->getInstanceBuffer(), pInstanceData, m_info.instanceDataSize, 0);
        }
    }

    void updateBuffer(Buffer* buffer, const void* pData, uint32_t size, uint32_t offset) {
        OGL_HANDLE handle = buffer->getHandle<OGL_HANDLE>();
        GL_CHECK(glBindBuffer(GL_COPY_WRITE_BUFFER, handle));
        void* dst{nullptr};
        GL_CHECK(dst = glMapBufferRange(GL_COPY_WRITE_BUFFER, offset, size,
                                        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        if (nullptr == dst) {
            GL_CHECK(glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, pData));
            return;
        } else {
            memcpy(dst, pData, size);
        }
        GL_CHECK(glUnmapBuffer(GL_COPY_WRITE_BUFFER));
        GL_CHECK(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
    }

private:
    GL3InputAssembler* m_input{nullptr};
    std::vector<uint8_t> m_vertexData;
    std::vector<uint8_t> m_indexData;
    std::vector<uint8_t> m_instanceData;
    UpdateAssemblerInfo m_info;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE