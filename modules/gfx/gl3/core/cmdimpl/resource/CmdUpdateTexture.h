#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdUpdateTexture : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::UPDATE_TEXUTRE;

public:
    CmdUpdateTexture(GL3CommandBuffer& cmdBuf)
        : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdUpdateTexture() {}

    void init(Texture* texture, const void* pData, uint32_t size,
              uint32_t width, uint32_t height) {
        m_texture = texture;
        m_texture->addRef();

        m_data.resize(size);

        memcpy(m_data.data(), pData, size);
        m_width  = width;
        m_height = height;
    }

    virtual void reset() override {
        m_texture->delRef();
        m_texture = nullptr;
        m_data.swap(std::vector<uint8_t>());
    }
    virtual void execute(gl3::GLContext* context) override {}

private:
    Texture* m_texture{nullptr};
    std::vector<uint8_t> m_data;
    uint32_t m_width;
    uint32_t m_height;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE