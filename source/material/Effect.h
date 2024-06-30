#pragma once

#include "common/Object.h"

BEGIN_NS_SCENCE_GRAPH

class Effect : public Object {
    DECLARE_RUNTIME_CLASS(Effect)
public:
    Effect();
    virtual ~Effect();

    virtual const char* getVertexShader() const   = 0;
    virtual const char* getFragmentShader() const = 0;
};

END_NS_SCENCE_GRAPH