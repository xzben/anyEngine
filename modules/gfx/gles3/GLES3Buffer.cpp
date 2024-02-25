#include "GLES3Buffer.h"

BEGIN_GFX_NAMESPACE

GLES3Buffer::GLES3Buffer(const Device &device)
    : GfxObject(device, GfxObjectType::Buffer, OGL_NULL_HANDLE,
                [](const Device &device, OGL_HANDLE handle) {

                }) {}

END_GFX_NAMESPACE