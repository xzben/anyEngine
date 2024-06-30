#pragma once

#include <string>
#include <unordered_map>

#include "Pass.h"
#include "common/Object.h"
#include "sceneGraphDefine.h"
BEGIN_NS_SCENCE_GRAPH

class Technique : public Object {
    DECLARE_RUNTIME_CLASS(Technique)
public:
    Technique(const std::string& name);
    virtual ~Technique();

    void addPass(Pass* pass);
    bool removePass(const std::string& name);
    Pass* getPass(const std::string& name) const;
    void removeAllPasses();
    const std::unordered_map<std::string, Pass*>& getPasses() const { return m_passes; }

    const std::string& getName() const { return m_name; }

protected:
    const std::string m_name;
    std::unordered_map<std::string, Pass*> m_passes;  // passes in technique
};

END_NS_SCENCE_GRAPH