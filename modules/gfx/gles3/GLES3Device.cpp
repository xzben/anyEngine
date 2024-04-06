#include "GLES3Device.h"

BEGIN_GFX_NAMESPACE

bool GLES3Device::init(const DeviceInfo& info) { return true; }
GLES3Shader* GLES3Device::createShader() {
    return new GLES3Shader(*this->m_pMainContext);
}
END_GFX_NAMESPACE