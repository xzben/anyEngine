#pragma once

#include "ReflexClassBase.h"
#include "ReflexClassFieldBase.h"
#include "reflexClass.h"
#include "reflexDefine.h"

#define DECLARE_REFLEX_CLASS(CLS)                                                  \
public:                                                                            \
    static NS_REFLEX::ReflexClass<CLS> s_Reflex##CLS##Obj;                         \
    static NS_REFLEX::ReflexClassBase* GetREFLEX() { return &s_Reflex##CLS##Obj; } \
    NS_REFLEX::ReflexClassBase* GetReflex() { return &s_Reflex##CLS##Obj; }        \
                                                                                   \
private:
