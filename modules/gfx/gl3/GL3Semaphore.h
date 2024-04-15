#pragma once

#include "../base/Semaphore.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Semaphore : public Semaphore {
public:
    GL3Semaphore(gl3::GLContext& context);
    virtual ~GL3Semaphore() {}

private:
};

END_GFX_NAMESPACE