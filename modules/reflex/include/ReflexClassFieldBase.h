#pragma once

#include <functional>
#include <string>

#include "ReflexClassBase.h"
#include "reflexDefine.h"

BEGIN_NS_REFLEX

class ReflexClassFieldBase {
public:
    ReflexClassFieldBase(ReflexClassBase* cls, const char* name, uint32_t flag)
        : m_cls(cls), m_flags(flag), m_rawName(name) {
        m_name = "$" + cls->m_name + "$." + name;
    }

    const std::string& getName() { return m_name; }
    const std::string& getRawName() { return m_rawName; }

protected:
    std::string m_name;
    std::string m_rawName;
    ReflexClassBase* m_cls{nullptr};
    uint32_t m_flags;
};

END_NS_REFLEX