#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdUpdateBuffer : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::UPDATE_BUFFER;

public:
    CmdUpdateBuffer(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdUpdateBuffer() {}

    void init() {}

    virtual void reset() override {}
    virtual void execute() override {}

private:
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE