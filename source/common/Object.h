#pragma once
#include <string>

#include "rtti.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

#define RELEASE_OBJ(obj)  \
    if (obj != nullptr) { \
        obj->delRef();    \
        obj = nullptr;    \
    }

class Object {
    DECLARE_RUNTIME_CLASS(Object)
public:
    Object(const std::string& name) : m_name(name){};
    Object(){};

    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() { return m_name; }

    uint32_t getRef() { return m_refCount; }
    void addRef() { ++m_refCount; }
    uint32_t delRef() {
        --m_refCount;
        uint32_t curRef = m_refCount;
        if (curRef <= 0) {
            delete this;
        }

        return curRef;
    }

    template <class CHECK_CLS>
    bool isKindOf() {
        NS_RTTI::Runtime* needRuntime = CHECK_CLS::GetRuntime();
        NS_RTTI::Runtime* runtime     = this->getRuntime();

        while (true) {
            if (runtime == nullptr) break;

            if (runtime == needRuntime) return true;

            runtime = runtime->getBase();
        }

        return false;
    }

    template <class CLS>
    CLS* toCast() {
        return isKindOf<CLS>() ? dynamic_cast<CLS*>(this) : nullptr;
    }

protected:
    virtual ~Object() = default;

private:
    std::string m_name;

    int32_t m_refCount{1};
};

END_NS_SCENCE_GRAPH