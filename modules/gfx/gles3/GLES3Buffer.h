#pragma once

#include "../base/Buffer.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class Device;

class GLES3Buffer : public Buffer, public GfxObject<OGL_HANDLE> {
public:
    GLES3Buffer(const Device& device);
};
END_GFX_NAMESPACE