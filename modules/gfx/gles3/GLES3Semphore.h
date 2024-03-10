#pragma once

#include "../base/Semaphore.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Semphore : public Semaphore, public GfxObject {
public:
    GLES3Semphore(gles::GLContext& context);
    virtual ~GLES3Semphore() {}

private:
};

END_GFX_NAMESPACE