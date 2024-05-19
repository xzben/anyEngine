#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdNextSubPass : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::NEXT_SUBPASS;

public:
    CmdNextSubPass(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdNextSubPass() {}

    void init(CmdBeginRenderPass* beginPass) { m_beginPass = beginPass; }

    virtual void reset() override { m_beginPass = nullptr; }
    virtual void execute(gl3::GLContext* context) override { m_beginPass->nextSubPass(context); }

private:
    CmdBeginRenderPass* m_beginPass;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE