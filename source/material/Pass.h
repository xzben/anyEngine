#pragma once

#include <string>

#include "gfx.h"
#include "sceneGraphDefine.h"
BEGIN_NS_SCENCE_GRAPH

struct Pass {
    std::string name;

    gfx::PipelineState pipelineState;
};

END_NS_SCENCE_GRAPH