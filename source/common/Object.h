#pragma once
#include <string>

#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

#define RELEASE_OBJ(obj)  \
    if (obj != nullptr) { \
        obj->delRef();    \
        obj = nullptr;    \
    }

class Object {
public:
    Object(const std::string& name) : m_name(name){};
    Object(){};

    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() { return m_name; }

    void addRef() { ++m_refCount; }
    void delRef() {
        if (--m_refCount <= 0) {
            delete this;
        }
    }

protected:
    virtual ~Object() = default;

private:
    std::string m_name;

    int32_t m_refCount{1};
};

END_NS_SCENCE_GRAPH