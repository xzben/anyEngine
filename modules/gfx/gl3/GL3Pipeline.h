#pragma once

#include "../base/Pipeline.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Pipeline : public Pipeline {
public:
    GL3Pipeline(GL3Device& device);
    virtual ~GL3Pipeline();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE