#pragma once

#include "../base/InputAssembler.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3InputAssembler : public InputAssembler {
public:
    GL3InputAssembler(GL3Device& device);
    virtual ~GL3InputAssembler();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE