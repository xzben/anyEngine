#pragma once

#include <vector>

#include "Pass.h"
#include "sceneGraphDefine.h"
BEGIN_NS_SCENCE_GRAPH

class Technique {
public:
    Technique();
    virtual ~Technique();

protected:
    std::vector<Pass*> m_passes;  // passes in technique
};

END_NS_SCENCE_GRAPH