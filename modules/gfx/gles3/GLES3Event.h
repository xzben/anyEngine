#pragma once

#include "../base/Event.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Event : public Event {
public:
    GLES3Event(gles::GLContext& context);
    virtual ~GLES3Event() {}

private:
};

END_GFX_NAMESPACE