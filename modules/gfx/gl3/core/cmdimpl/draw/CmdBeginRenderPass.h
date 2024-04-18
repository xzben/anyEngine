#include "../CmdBase.h"

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
              const std::vector<ClearValue>& clearValues) {}

    virtual void reset() override {}
    virtual void execute() override {}

private:
    RenderPass* m_renderpass;
    std::vector<DrawSurface> m_attachments;
    std::vector<ClearValue> m_clearValues;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE