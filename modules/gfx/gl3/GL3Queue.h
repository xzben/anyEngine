#pragma once

#include "../base/Queue.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Queue : public Queue {
public:
    GL3Queue(gl3::GLContext& context);
    virtual ~GL3Queue() {}

private:
};

END_GFX_NAMESPACE