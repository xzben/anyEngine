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
    virtual void execute(gl3::GLContext* context) override {
        OGL_HANDLE fbo{OGL_NULL_HANDLE};
        context->allocFbo(1, &fbo);

        OGL_HANDLE textHandleSrc = m_src->getHandle<OGL_HANDLE>();
        auto& srcInfo            = m_src->getInfo();

        OGL_HANDLE textHandleDst = m_dst->getHandle<OGL_HANDLE>();
        auto& dstInfo            = m_dst->getInfo();

        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo));
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

        if (dstInfo.type == TextureType::TEX2D_ARRAY) {
            GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, textHandleDst));
            GL_CHECK(glCopyTexSubImage3D(
                GL_TEXTURE_2D_ARRAY, 0, m_info.dstOffset.x, m_info.dstOffset.y,
                m_info.dstLayerIndex, m_info.srcOffset.x, m_info.srcOffset.y,
                m_info.range.x, m_info.range.y));
            GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
        } else {
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, textHandleDst));
            GL_CHECK(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, m_info.dstOffset.x,
                                         m_info.dstOffset.y, m_info.srcOffset.x,
                                         m_info.srcOffset.y, m_info.range.x,
                                         m_info.range.y));
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
        }

        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
        context->freeFbo(1, &fbo);
    }

private:
    Texture* m_src{nullptr};
    Texture* m_dst{nullptr};
    TextureCopyInfo m_info;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE