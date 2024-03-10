#pragma once

#include "../base/Pipeline.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Pipeline : public Pipeline, public GfxObject {
public:
    GLES3Pipeline(gles::GLContext& context);
    virtual ~GLES3Pipeline() {}

private:
};

END_GFX_NAMESPACE