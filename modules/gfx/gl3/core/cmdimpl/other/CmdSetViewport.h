#pragma once

#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdSetViewport : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::SET_VIEWPORT;

public:
    CmdSetViewport(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdSetViewport() {}

    void init(float x, float y, float width, float height) {
        m_viewport = {x, y, width, height};
    }

    virtual void reset() override {}
    virtual void execute() override {}

private:
    Viewport m_viewport;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE