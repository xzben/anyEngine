#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "common/Object.h"

BEGIN_NS_SCENCE_GRAPH

class Pass;
class Technique;
class Shader;
class Texture;
class Material : public Object {
    DECLARE_RUNTIME_CLASS(Material)
public:
    Material();
    virtual ~Material();

    void addTechnique(Technique* technique);
    bool setCurrentTechnique(const std::string& name);
    bool removeTechnique(const std::string& name);
    void removeAllTechniques();
    Technique* getCurrentTechnique() const { return m_currentTechnique; }
    const std::unordered_map<std::string, Technique*>& getTechniques() const {
        return m_techniques;
    }

protected:
    std::unordered_map<std::string, Technique*> m_techniques;
    Technique* m_currentTechnique{nullptr};
};

END_NS_SCENCE_GRAPH