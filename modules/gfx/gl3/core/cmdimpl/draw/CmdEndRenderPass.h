#pragma once

#include "../CmdBase.h"
#include "CmdBeginRenderPass.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdEndRenderPass : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::END_RENDERPASS;

public:
    CmdEndRenderPass(GL3CommandBuffer& cmdBuf)
        : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdEndRenderPass() {}

    void init(CmdBeginRenderPass* beginPass) { m_beginPass = beginPass; }

    virtual void reset() override { m_beginPass = nullptr; }
    virtual void execute(gl3::GLContext* context) override {}

private:
    CmdBeginRenderPass* m_beginPass;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE