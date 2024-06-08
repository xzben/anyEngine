#pragma once

#include <functional>
#include <string>

#include "reflexDefine.h"

BEGIN_NS_REFLEX

class ReflexClassBase {
public:
    friend class ReflexClassFieldBase;

    ReflexClassBase(const char* name, ReflexClassBase* parent = nullptr);
    virtual ~ReflexClassBase() = default;

    virtual bool serialize()    = 0;
    virtual bool deserialize()  = 0;
    virtual void* newInstance() = 0;

    virtual void getFields(std::vector<ReflexClassFieldBase*>& fields)                 = 0;
    virtual void foreachFields(std::function<void(ReflexClassFieldBase* field)>& func) = 0;

    const std::string& getName() { return m_name; }

protected:
    const std::string& m_name;
    ReflexClassBase* m_parent{nullptr};
};

END_NS_REFLEX