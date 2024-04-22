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
    virtual void execute(gl3::GLContext* context) override {
        OGL_HANDLE fboR = context->getFbo(0);
        OGL_HANDLE fboW = context->getFbo(1);

        OGL_HANDLE textHandleSrc = m_src->getHandle<OGL_HANDLE>();
        auto& srcInfo            = m_src->getInfo();

        OGL_HANDLE textHandleDst = m_dst->getHandle<OGL_HANDLE>();
        auto& dstInfo            = m_dst->getInfo();

        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, fboR));
        if (srcInfo.type == TextureType::TEX2D_ARRAY) {
            GL_CHECK(glFramebufferTextureLayer(
                GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textHandleSrc, 0,
                m_info.srcLayerIndex));
        } else {
            GL_CHECK(glFramebufferTexture2D(GL_READ_FRAMEBUFFER,
                                            GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                            textHandleSrc, 0));
        }
        assert(glCheckFramebufferStatus(GL_READ_FRAMEBUFFER)
               == GL_FRAMEBUFFER_COMPLETE);

        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboW));
        if (dstInfo.type == TextureType::TEX2D_ARRAY) {
            GL_CHECK(glFramebufferTextureLayer(
                GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textHandleDst, 0,
                m_info.dstLayerIndex));
        } else {
            GL_CHECK(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                                            GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                            textHandleDst, 0));
        }
        assert(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)
               == GL_FRAMEBUFFER_COMPLETE);

        GL_CHECK(glBlitFramebuffer(m_info.srcOffset.x, m_info.srcOffset.y,
                                   m_info.srcOffset.x + m_info.srcRange.x,
                                   m_info.srcOffset.y + m_info.srcRange.y,

                                   m_info.dstOffset.x, m_info.dstOffset.y,
                                   m_info.dstOffset.x + m_info.dstRange.x,
                                   m_info.dstOffset.y + m_info.srcRange.y,
                                   GL_COLOR_BUFFER_BIT, GL_LINEAR));

        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    }

private:
    TextureBliteInfo m_info;
    Texture* m_src{nullptr};
    Texture* m_dst{nullptr};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE