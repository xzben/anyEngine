#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdCopyBuffer : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::COPY_BUFFER;

public:
    CmdCopyBuffer(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdCopyBuffer() {}

    void init(Buffer* src, Buffer* dst, uint32_t size, uint32_t srcOffset,
              uint32_t dstOffset) {
        m_src = src;
        m_src->addRef();

        m_dst = dst;
        m_dst->addRef();

        m_srcOffset = srcOffset;
        m_dstOffset = dstOffset;
        m_size      = size;
    }

    virtual void reset() override {
        m_src->delRef();
        m_src = nullptr;

        m_dst->delRef();
        m_dst = nullptr;
    }
    virtual void execute() override {}

private:
    Buffer* m_src{nullptr};
    Buffer* m_dst{nullptr};
    uint32_t m_size{0};
    uint32_t m_srcOffset{0};
    uint32_t m_dstOffset{0};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE