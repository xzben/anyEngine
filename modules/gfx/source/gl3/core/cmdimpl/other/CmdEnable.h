#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdEnable : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::ENABLE;

    static GLenum getGlStateCap(RenderState state) {
        GLenum cap = 0;
        if (hasFlag(state, RenderState::SCISSOR)) cap |= GL_SCISSOR_TEST;

        return cap;
    }

public:
    CmdEnable(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdEnable() {}

    void init(RenderState state) { m_state = state; }

    virtual void reset() override {}
    virtual void execute(gl3::GLContext* context) override {
        GLenum cap = getGlStateCap(m_state);
        GL_CHECK(glEnable(cap));
    }

private:
    RenderState m_state;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE