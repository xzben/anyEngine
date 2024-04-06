#pragma once

#include "../base/Device.h"
#include "GLES3Shader.h"
#include "core/GLContext.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Device : public Device {
public:
    virtual bool init(const DeviceInfo& info) override;
    virtual GLES3Shader* createShader() override;

private:
    std::unique_ptr<gles::GLContext> m_pMainContext;
};

END_GFX_NAMESPACE