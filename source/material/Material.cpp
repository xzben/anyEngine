#include "Material.h"

#include "Pass.h"
#include "Technique.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Material)

Material::Material() {}

Material::~Material() { removeAllTechniques(); }

void Material::addTechnique(Technique* technique) {
    const std::string& name = technique->getName();
    removeTechnique(name);

    technique->addRef();
    m_techniques[name] = technique;
}

bool Material::setCurrentTechnique(const std::string& name) {
    auto it = m_techniques.find(name);
    if (it == m_techniques.end()) {
        return false;
    }

    m_currentTechnique = it->second;
    return true;
}

bool Material::removeTechnique(const std::string& name) {
    auto it = m_techniques.find(name);
    if (it == m_techniques.end()) {
        return false;
    }

    it->second->delRef();
    m_techniques.erase(it);
    return true;
}

void Material::removeAllTechniques() {
    for (auto& pair : m_techniques) {
        pair.second->delRef();
    }
    m_techniques.clear();
}

END_NS_SCENCE_GRAPH