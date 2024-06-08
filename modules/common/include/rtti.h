#pragma once
#include <string>
#include <type_traits>

#define BEGIN_NS_RTTI namespace rtti {
#define END_NS_RTTI }
#define NS_RTTI rtti

BEGIN_NS_RTTI

class Runtime final {
public:
    Runtime(const char* name, Runtime* pParent = nullptr) : m_name(name), m_parent(pParent) {}
    Runtime() = default;

    Runtime* getParent() { return m_parent; }
    const std::string& getName() { return m_name; }

protected:
    Runtime* m_parent{nullptr};
    std::string m_name;
};

class EmptyRuntimeSuper {
public:
    static rtti::Runtime* GetRuntime() { return nullptr; }
};

END_NS_RTTI;  // namespace rtti

using RuntimeThisType = NS_RTTI::EmptyRuntimeSuper;

#define DECLARE_RUNTIME_CLASS(CLS)                                         \
public:                                                                    \
    using RuntimeSuperType = RuntimeThisType;                              \
    using RuntimeThisType  = CLS;                                          \
    static NS_RTTI::Runtime s_Runtime##CLS##Obj;                           \
    static NS_RTTI::Runtime* GetRuntime() { return &s_Runtime##CLS##Obj; } \
    NS_RTTI::Runtime* getRuntime() { return &s_Runtime##CLS##Obj; }        \
                                                                           \
private:

#define IMPLEMENT_RUNTIME_CLASS(CLS) \
    NS_RTTI::Runtime CLS::s_Runtime##CLS##Obj(#CLS, CLS::RuntimeSuperType::GetRuntime());

#define DECLARE_RUNTIME_TEMPLATE_CLASS(CLS, T)                             \
public:                                                                    \
    using RuntimeSuperType = RuntimeThisType;                              \
    using RuntimeThisType  = CLS<T>;                                       \
    static NS_RTTI::Runtime s_Runtime##CLS##Obj;                           \
    static NS_RTTI::Runtime* GetRuntime() { return &s_Runtime##CLS##Obj; } \
    NS_RTTI::Runtime* getRuntime() { return &s_Runtime##CLS##Obj; }        \
                                                                           \
private:

#define IMPLEMENT_RUNTIME_TEMPLATE_CLASS(CLS, T) \
    template <T>                                 \
    NS_RTTI::Runtime CLS<T>::s_Runtime##CLS##Obj(#CLS<T>, CLS::RuntimeSuperType::GetRuntime());