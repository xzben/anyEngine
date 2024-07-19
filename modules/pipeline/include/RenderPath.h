#pragma once

#include <optional>
#include <string>
#include <vector>

#include "PipelineTypes.h"
#include "gfx.h"
#include "pipelineMacro.h"
BEGIN_PIPELINE_NAMESPACE

enum class RenderCommandSortMode { NONE = 0, BACK_TO_FRONT, FRONT_TO_BACK };

struct RenderCommand {
    std::string passName;
    std::string name;
    std::vector<RenderTargetDesc> innerTargets;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<gfx::ClearValue> clearValues;
    RenderCommandSortMode sortMode;
};

class RenderPath {
public:
    RenderPath() = default;

    void addTarget(const RenderTargetDesc& targetDesc);
    void setTarget(size_t index, const RenderTargetDesc& targetDesc);
    void insertTarget(size_t index, const RenderTargetDesc& targetDesc);
    void removeTarget(const std::string& name);
    void removeTarget(size_t index);
    const RenderTargetDesc* getTarget(const std::string& name) const;

    void addCommand(const RenderCommand& command);
    void insertCommand(size_t index, const RenderCommand& command);
    void setCommand(size_t index, const RenderCommand& command);
    void removeCommand(size_t index);

    const std::vector<RenderTargetDesc>& getRenderTargets() const { return m_renderTargets; }
    const std::vector<RenderCommand>& getRenderCommands() const { return m_renderCommands; }

private:
    std::vector<RenderTargetDesc> m_renderTargets;
    std::vector<RenderCommand> m_renderCommands;
};

END_PIPELINE_NAMESPACE