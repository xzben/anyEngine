#include "../CmdBase.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdGenerateMipmap : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::GENERATE_MIPMAP;

public:
    CmdGenerateMipmap(GL3CommandBuffer& cmdBuf)
        : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdGenerateMipmap() {}

    void init() {}

    virtual void reset() override {}
    virtual void execute() override {}

private:
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE