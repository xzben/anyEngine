#pragma once

#include "framegraph/FrameGraph.h"
#include "pipeline.h"
BEGIN_PIPELINE_NAMESPACE

class RenderPipelineImpl : public RenderPipeline {
public:
    RenderPipelineImpl(gfx::Device& device, RenderPassMgr& renderPassMgr);
    virtual ~RenderPipelineImpl() override;

    virtual void setup(RenderPath* path, RenderTarget* target) override;
    virtual void render(const std::vector<RenderNode*>& nodes, const RenderView& view) override;

private:
    gfx::Device& m_device;
    RenderPassMgr& m_renderPassMgr;
    NS_FG::FrameGraph m_frameGraph;
};
END_PIPELINE_NAMESPACE