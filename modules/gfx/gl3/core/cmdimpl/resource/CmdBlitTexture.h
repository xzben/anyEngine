#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdBlitTexture : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::BLIT_TEXTURE;

public:
    CmdBlitTexture(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdBlitTexture() {}

    void init(Texture* src, Texture* dst, const TextureBliteInfo& info) {
        m_info = info;
        m_src  = src;
        m_dst  = dst;

        m_dst->addRef();
        m_src->addRef();
    }

    virtual void reset() override {
        m_dst->delRef();
        m_dst = nullptr;
        m_src->delRef();
        m_src = nullptr;
    }
    virtual void execute() override {}

private:
    TextureBliteInfo m_info;
    Texture* m_src{nullptr};
    Texture* m_dst{nullptr};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE