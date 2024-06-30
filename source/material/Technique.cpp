#include "Technique.h"

BEGIN_NS_SCENCE_GRAPH
IMPLEMENT_RUNTIME_CLASS(Technique)

Technique::Technique(const std::string& name) : m_name(name) {}
Technique::~Technique() { removeAllPasses(); }

void Technique::addPass(Pass* pass) {
    auto& name = pass->getName();
    removePass(name);

    pass->addRef();
    m_passes[name] = pass;
}

bool Technique::removePass(const std::string& name) {
    auto it = m_passes.find(name);
    if (it == m_passes.end()) {
        return false;
    }

    it->second->delRef();
    m_passes.erase(it);
    return true;
}

Pass* Technique::getPass(const std::string& name) const {
    auto it = m_passes.find(name);
    if (it == m_passes.end()) {
        return nullptr;
    }

    return it->second;
}

void Technique::removeAllPasses() {
    for (auto& pair : m_passes) {
        pair.second->delRef();
    }
    m_passes.clear();
}

END_NS_SCENCE_GRAPH