#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdClearTexture : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::CLEAR_TEXTURE;

public:
    CmdClearTexture(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdClearTexture() {}

    void init(Texture* dst, const TextureSubInfo& info, const Color& color) {
        m_info  = info;
        m_dst   = dst;
        m_color = color;
        m_dst->addRef();
    }

    virtual void reset() override {
        m_dst->delRef();
        m_dst = nullptr;
    }
    virtual void execute(gl3::GLContext* context) override {
        OGL_HANDLE fbo = OGL_NULL_HANDLE;
        context->allocFbo(1, &fbo);

        OGL_HANDLE textHandleDst = m_dst->getHandle<OGL_HANDLE>();
        auto& dstInfo            = m_dst->getInfo();

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
        if (dstInfo.type == TextureType::TEX2D_ARRAY) {
            GL_CHECK(glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textHandleDst,
                                               0, m_info.layerIndex));
        } else {
            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                            textHandleDst, 0));
        }
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        bool needScissor = false;
        if (m_info.range.x != dstInfo.width || m_info.range.y != dstInfo.height) {
            GL_CHECK(glScissor(m_info.offset.x, m_info.offset.y, m_info.range.x, m_info.range.y));
            GL_CHECK(glEnable(GL_SCISSOR_TEST));
            needScissor = true;
        }
        GL_CHECK(glClearColor(m_color.r, m_color.r, m_color.b, m_color.a));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
        if (needScissor) {
            GL_CHECK(glDisable(GL_SCISSOR_TEST));
        }
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        context->freeFbo(1, &fbo);
    }

private:
    TextureSubInfo m_info;
    Texture* m_dst{nullptr};
    Color m_color{0.f, 0.f, 0.f, 0.f};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE