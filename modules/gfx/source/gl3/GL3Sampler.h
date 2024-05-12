#pragma once

#include "base/Sampler.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Sampler : public Sampler {
public:
    GL3Sampler(GL3Device& device, const SamplerInfo& info);
    virtual ~GL3Sampler();

protected:
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }

private:
    SamplerInfo m_info;
    GL3Device& m_device;
    OGL_HANDLE m_handle{OGL_NULL_HANDLE};
};

END_GFX_NAMESPACE