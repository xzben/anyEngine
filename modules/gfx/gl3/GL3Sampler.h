#pragma once

#include "../base/Sampler.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Sampler : public Sampler {
public:
    GL3Sampler(GL3Device& device);
    virtual ~GL3Sampler();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE