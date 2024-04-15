#pragma once

#include "../base/Queue.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Queue : public Queue {
public:
    GLES3Queue(gles::GLContext& context);
    virtual ~GLES3Queue() {}

private:
};

END_GFX_NAMESPACE