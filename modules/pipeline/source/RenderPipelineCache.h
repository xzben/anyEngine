#pragma once

#include "RenderPipeline.h"
#include "pipeline_common.h"

BEGIN_PIPELINE_NAMESPACE

class RenderPipelineCache {
public:
    RenderPipelineCache();

    void addPipeline(const std::string& name, RenderPipeline* pipeline) {
        m_pipelines[name] = pipeline;
    }
    RenderPipeline* getPipeline(const std::string& name) {
        auto it = m_pipelines.find(name);
        if (it == m_pipelines.end()) {
            return nullptr;
        }

        return it->second;
    }

private:
    std::unordered_map<std::string, RenderPipeline*> m_pipelines;
};
END_PIPELINE_NAMESPACE