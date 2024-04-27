#pragma once

#include "../CmdBase.h"
#include "gl3/GL3RenderPass.h"
BEGIN_GFX_NAMESPACE
class RenderPass;
class DrawSurface;
BEGIN_GL3_CORE_NAMESPACE

class CmdBeginRenderPass : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::BEGIN_RENDERPASS;

public:
    CmdBeginRenderPass(GL3CommandBuffer& cmdBuf)
        : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdBeginRenderPass() {}

    void init(RenderPass* renderpass,
              const std::vector<DrawSurface*>& attachments,
              const std::vector<ClearValue>& clearValues) {
        m_renderpass = dynamic_cast<GL3RenderPass*>(renderpass);
        m_renderpass->addRef();

        m_attachments = attachments;
        for (auto& att : m_attachments) {
            att->addRef();
        }
        m_clearValues = clearValues;
    }

    virtual void reset() override {
        m_renderpass->delRef();
        m_renderpass = nullptr;

        for (auto& att : m_attachments) {
            att->delRef();
        }
        m_attachments.clear();
        m_clearValues.clear();
    }
    virtual void execute(gl3::GLContext* context) {
        m_curPassIndex = 0;
        switchSubpass(context, m_curPassIndex);
    }
    void nextSubPass(gl3::GLContext* context) {
        m_curPassIndex++;
        switchSubpass(context, m_curPassIndex);
    }
    void switchSubpass(gl3::GLContext* context, uint32_t index);
    void endRenderPass(gl3::GLContext* context);

protected:
    void bindSubpassFbo(gl3::GLContext* context, uint32_t index);
    void clearSubpassFbo(gl3::GLContext* context, uint32_t index);

private:
    OGL_HANDLE m_fbo{OGL_NULL_HANDLE};
    GL3RenderPass* m_renderpass;
    std::vector<DrawSurface*> m_attachments;
    std::vector<ClearValue> m_clearValues;
    uint32_t m_curPassIndex{0};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE