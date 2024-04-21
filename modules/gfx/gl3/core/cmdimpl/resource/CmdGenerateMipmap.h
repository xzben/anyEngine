#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdGenerateMipmap : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::GENERATE_MIPMAP;

public:
    CmdGenerateMipmap(GL3CommandBuffer& cmdBuf)
        : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdGenerateMipmap() {}

    void init(Texture* texture, uint32_t mipLevels) {
        m_texture = texture;
        m_texture->addRef();
        m_mipLevels = mipLevels;
    }

    virtual void reset() override {
        m_texture->delRef();
        m_texture = nullptr;
    }
    virtual void execute() override {}

private:
    Texture* m_texture{nullptr};
    uint32_t m_mipLevels;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE