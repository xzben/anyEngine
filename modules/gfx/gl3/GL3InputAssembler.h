#pragma once

#include "../base/InputAssembler.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3InputAssembler : public InputAssembler {
public:
    GL3InputAssembler(gl3::GLContext& context);
    virtual ~GL3InputAssembler() {}

private:
};

END_GFX_NAMESPACE