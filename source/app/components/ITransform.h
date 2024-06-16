#pragma once

#include "../Component.h"
#include "glm/glm.hpp"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

class ITransform {
public:
    ITransform()          = default;
    virtual ~ITransform() = default;

    virtual void updateModel()                        = 0;
    virtual const glm::mat4& getModeTransform() const = 0;
};

END_NS_SCENCE_GRAPH