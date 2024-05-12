#pragma once

#include "../CmdBase.h"
#include "CmdEnable.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdDisable : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::DISABLE;

public:
    CmdDisable(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdDisable() {}

    void init(RenderState state) { m_state = state; }

    virtual void reset() override {}

    virtual void execute(gl3::GLContext* context) override {
        GLenum cap = CmdEnable::getGlStateCap(m_state);
        GL_CHECK(glDisable(cap));
    }

private:
    RenderState m_state;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE