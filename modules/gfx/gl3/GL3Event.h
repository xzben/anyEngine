#pragma once

#include "../base/Event.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Event : public Event {
public:
    GL3Event(gl3::GLContext& context);
    virtual ~GL3Event() {}

private:
};

END_GFX_NAMESPACE