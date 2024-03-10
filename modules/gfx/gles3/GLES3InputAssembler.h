#pragma once

#include "../base/InputAssembler.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3InputAssembler : public InputAssembler, public GfxObject {
public:
    GLES3InputAssembler(gles::GLContext& context);
    virtual ~GLES3InputAssembler() {}

private:
};

END_GFX_NAMESPACE