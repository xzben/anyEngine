#pragma once

#include <string>
#include <vector>

#include "pipelineMacro.h"
#include "pipelineTypes.h"

BEGIN_PIPELINE_NAMESPACE

class RenderSubpass {};

class RenderPass {
public:
    RenderPass(const std::string& name) : m_name(name) {}
    const std::string& getName() const { return m_name; }

    const std::vector<RenderSubpass>& getSubpasses() const { return m_subpasses; }

    const std::vector<RenderTargetDesc>& inputs() { return m_inputAttachments; }
    const std::vector<RenderTargetDesc>& outputs() { return m_ouputAttachments; }

private:
    std::string m_name;
    std::vector<RenderTargetDesc> m_inputAttachments;
    std::vector<RenderTargetDesc> m_ouputAttachments;
    std::vector<RenderSubpass> m_subpasses;
};

END_PIPELINE_NAMESPACE