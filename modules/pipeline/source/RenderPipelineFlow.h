#pragma once

#include "RenderPipeline.h"
#include "pipeline_common.h"

BEGIN_PIPELINE_NAMESPACE

class RenderPipelineFlow {
public:
    void addPipeline(const std::string& name, RenderPipeline* pipeline);
    RenderPipeline* getPipeline(const std::string& name);

private:
    std::unordered_map<std::string, RenderPipeline*> m_pipelines;
};
END_PIPELINE_NAMESPACE