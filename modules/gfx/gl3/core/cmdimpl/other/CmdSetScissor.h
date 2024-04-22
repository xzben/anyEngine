#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdSetScissor : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::SET_SCISSOR;

public:
    CmdSetScissor(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdSetScissor() {}

    void init(float x, float y, float width, float height) {
        m_viewport = {x, y, width, height};
    }

    virtual void reset() override {}
    virtual void execute(gl3::GLContext* context) override {
        GL_CHECK(glScissor(m_viewport.x, m_viewport.y, m_viewport.width,
                           m_viewport.height));
    }

private:
    Viewport m_viewport;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE