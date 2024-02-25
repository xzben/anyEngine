#pragma once

#include "../base/Buffer.h"
#include "gles_common.h"
BEGIN_GFX_NAMESPACE

class GLES3Buffer : public Buffer, public GfxObject<OGL_HANDLE> {};
END_GFX_NAMESPACE