#include "CmdBeginRenderPass.h"

#include "../../GLContext.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

void CmdBeginRenderPass::endRenderPass(gl3::GLContext* context) {
    context->freeFbo(1, &m_fbo);
    m_fbo = OGL_NULL_HANDLE;
}

void CmdBeginRenderPass::switchSubpass(gl3::GLContext* context,
                                       uint32_t index) {
    bindSubpassFbo(context, index);
    clearSubpassFbo(context, index);
}

void CmdBeginRenderPass::bindSubpassFbo(gl3::GLContext* context,
                                        uint32_t index) {
    const auto& attachments = m_renderpass->getAttachments();
    const auto& subpasses   = m_renderpass->getSubpasses();

    assert(subpasses.size() > index);
    assert(attachments.size() == m_attachments.size());

    const auto& pass = subpasses[index];

    if (m_fbo == OGL_NULL_HANDLE) {
        context->allocFbo(1, &m_fbo);
    }

    auto target = GL_FRAMEBUFFER;
    GL_CHECK(glBindFramebuffer(target, m_fbo));

    for (int i = 0; i < pass.colorAttachments.size(); i++) {
        const auto& colorAtt   = pass.colorAttachments[i];
        const auto& drawTarget = m_attachments[colorAtt.attachment];
        auto texture           = drawTarget->getTexture();
        const auto& info       = texture->getInfo();
        auto textureHandle     = texture->getHandle<OGL_HANDLE>();
        switch (info.type) {
            case TextureType::TEX2D: {
                GL_CHECK(
                    glFramebufferTexture2D(target, GL_COLOR_ATTACHMENT0 + i,
                                           GL_TEXTURE_2D, textureHandle, 0));
                break;
            }
            case TextureType::TEX2D_ARRAY: {
                GL_CHECK(glFramebufferTextureLayer(
                    target, GL_COLOR_ATTACHMENT0 + i, textureHandle, 0,
                    drawTarget->getLayerIndex()));
                break;
            }
            default: {
                CCASSERT(false, "unknown texture type:%d", info.type);
                break;
            }
        }
    }

    for (int i = 0; i < pass.depthStencilAttachments.size(); i++) {
        const auto& depthAtt   = pass.depthStencilAttachments[i];
        const auto& drawTarget = m_attachments[depthAtt.attachment];
        auto texture           = drawTarget->getTexture();
        const auto& info       = texture->getInfo();
        auto textureHandle     = texture->getHandle<OGL_HANDLE>();
        GL_CHECK(glFramebufferTexture2D(target, GL_DEPTH_STENCIL_ATTACHMENT,
                                        GL_TEXTURE_2D, textureHandle, 0));
    }
}

void CmdBeginRenderPass::clearSubpassFbo(gl3::GLContext* context,
                                         uint32_t index) {
    const auto& attachments = m_renderpass->getAttachments();
    const auto& subpasses   = m_renderpass->getSubpasses();

    assert(subpasses.size() > index);
    assert(attachments.size() == m_attachments.size());

    const auto& pass = subpasses[index];

    for (int i = 0; i < pass.colorAttachments.size(); i++) {
        const auto& colorAtt = pass.colorAttachments[i];
        const auto& att      = attachments[colorAtt.attachment];

        if (att.load_op == LoadOp::CLEAR) {
            const auto& clearValue = m_clearValues[colorAtt.attachment];
            GL_CHECK(glClearBufferfv(GL_COLOR, i, clearValue.color.values));
        }
    }

    for (int i = 0; i < pass.depthStencilAttachments.size(); i++) {
        const auto& depthAtt = pass.depthStencilAttachments[i];
        const auto& att      = attachments[depthAtt.attachment];

        if (att.load_op == LoadOp::CLEAR) {
            const auto& clearValue = m_clearValues[depthAtt.attachment];
            GL_CHECK(glClearBufferfi(GL_DEPTH_STENCIL, i,
                                     clearValue.depthStencil.depth,
                                     clearValue.depthStencil.stencil));
        }
    }
}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE