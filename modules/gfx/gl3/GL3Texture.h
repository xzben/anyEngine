#pragma once

#include "../base/Texture.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Texture : public Texture {
public:
    GL3Texture(GL3Device& device);
    virtual ~GL3Texture();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE