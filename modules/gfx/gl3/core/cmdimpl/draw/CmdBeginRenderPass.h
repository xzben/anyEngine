#pragma once

#include "../CmdBase.h"
#include "gl3/GL3RenderPass.h"
BEGIN_GFX_NAMESPACE
class RenderPass;
BEGIN_GL3_CORE_NAMESPACE

class CmdBeginRenderPass : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::BEGIN_RENDERPASS;

public:
    CmdBeginRenderPass(GL3CommandBuffer& cmdBuf)
        : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdBeginRenderPass() {}

    void init(RenderPass* renderpass,
              const std::vector<DrawSurface>& attachments,
              const std::vector<ClearValue>& clearValues) {
        m_renderpass = renderpass;
        m_renderpass->addRef();

        m_attachments = attachments;
        for (auto& att : m_attachments) {
            att.addRef();
        }
        m_clearValues = clearValues;
    }

    virtual void reset() override {
        m_renderpass->delRef();
        m_renderpass = nullptr;

        for (auto& att : m_attachments) {
            att.delRef();
        }
        m_attachments.clear();
        m_clearValues.clear();
    }
    virtual void execute(gl3::GLContext* context) override {}

private:
    RenderPass* m_renderpass;
    std::vector<DrawSurface> m_attachments;
    std::vector<ClearValue> m_clearValues;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE