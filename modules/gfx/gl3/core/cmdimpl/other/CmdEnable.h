#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdEnable : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::ENABLE;

public:
    CmdEnable(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdEnable() {}

    void init(RenderState state) { m_state = state; }

    virtual void reset() override {}
    virtual void execute() override {}

private:
    RenderState m_state;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE