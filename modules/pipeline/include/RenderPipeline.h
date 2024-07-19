#pragma once
#include "RenderPassMgr.h"
#include "RenderPath.h"
#include "pipelineMacro.h"
#include "pipelineTypes.h"
BEGIN_PIPELINE_NAMESPACE

class RenderTarget;

class RenderPipeline {
public:
    static RenderPipeline* create(gfx::Device& device, RenderPassMgr& renderPassMgr);

    virtual ~RenderPipeline() = default;

    virtual void setup(RenderPath* path, RenderTarget* target)                         = 0;
    virtual void render(const std::vector<RenderNode*>& nodes, const RenderView& view) = 0;

protected:
    RenderPipeline() = default;
};

END_PIPELINE_NAMESPACE