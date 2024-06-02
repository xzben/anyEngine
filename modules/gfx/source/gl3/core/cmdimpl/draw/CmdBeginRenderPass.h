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
    CmdBeginRenderPass(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdBeginRenderPass() { reset(); }

    void init(const BeginRenderPassInfo& info) {
        m_renderpass = dynamic_cast<GL3RenderPass*>(info.renderPass);
        m_renderpass->addRef();

        m_attachments.resize(info.surfaceCount);
        for (uint32_t i = 0; i < info.surfaceCount; i++) {
            m_attachments[i] = info.pSurfaces[i];
            m_attachments[i]->addRef();
        }

        if (info.pClearValues != nullptr) {
            m_clearValues.resize(info.surfaceCount);
            for (uint32_t i = 0; i < info.surfaceCount; i++) {
                m_clearValues[i] = info.pClearValues[i];
            }
        }

        m_viewport = info.viewport;
    }

    virtual void reset() override {
        if (m_renderpass) {
            m_renderpass->delRef();
            m_renderpass = nullptr;
        }

        for (auto& att : m_attachments) {
            att->delRef();
        }
        m_attachments.clear();
        m_clearValues.clear();
    }

    void execute(gl3::GLContext* context) override {
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
    Viewport m_viewport;
    uint32_t m_curPassIndex{0};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE