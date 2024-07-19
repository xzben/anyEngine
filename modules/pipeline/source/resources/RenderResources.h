#pragma once

#include "gfx.h"
#include "pipelineMacro.h"
#include "pipelineTypes.h"

BEGIN_PIPELINE_NAMESPACE

class RenderTexture {
public:
    RenderTexture(gfx::Device* device, const RenderTargetDesc& desc, const Size& size) {}
};

class RenderBuffer {
public:
    RenderBuffer(gfx::Device* device, uint32_t size) {}
};

END_PIPELINE_NAMESPACE