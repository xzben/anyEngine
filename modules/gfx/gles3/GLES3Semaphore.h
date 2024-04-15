#pragma once

#include "../base/Semaphore.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Semaphore : public Semaphore {
public:
    GLES3Semaphore(gles::GLContext& context);
    virtual ~GLES3Semaphore() {}

private:
};

END_GFX_NAMESPACE