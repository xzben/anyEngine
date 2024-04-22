#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdCopyTexture : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::COPY_TEXTURE;

public:
    CmdCopyTexture(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}

    virtual ~CmdCopyTexture() {}

    void init(Texture* src, Texture* dst, const TextureCopyInfo& info) {
        m_src = src;
        m_src->addRef();

        m_dst = dst;
        m_dst->addRef();
        m_info = info;
    }

    virtual void reset() override {
        m_src->delRef();
        m_src = nullptr;

        m_dst->delRef();
        m_dst = nullptr;
    }
    virtual void execute(gl3::GLContext* context) override {}

private:
    Texture* m_src{nullptr};
    Texture* m_dst{nullptr};
    TextureCopyInfo m_info;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE