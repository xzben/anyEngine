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

    void init(Buffer* buffer, const void* pData, uint32_t size,
              uint32_t offset) {
        m_buffer = buffer;
        m_buffer->addRef();

        m_data.resize(size);
        memcpy(m_data.data(), pData, size);

        m_offset = offset;
    }

    virtual void reset() override {
        m_buffer->delRef();
        m_buffer = nullptr;
        m_data.swap(std::vector<uint8_t>());
    }

    virtual void execute() override {}

private:
    Buffer* m_buffer{nullptr};
    std::vector<uint8_t> m_data;
    uint32_t m_offset;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE