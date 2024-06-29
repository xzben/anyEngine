#pragma once
#include <string>
#include <vector>

#include "common/Object.h"

BEGIN_NS_SCENCE_GRAPH

class Pass;
class Technique;
class Shader;
class Texture;
class Material : public Object {
public:
    Material();
    virtual ~Material();

protected:
    std::vector<Technique*> m_techniques;
};

END_NS_SCENCE_GRAPH