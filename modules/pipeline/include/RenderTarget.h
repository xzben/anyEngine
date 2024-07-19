#pragma once

#include "pipelineMacro.h"
#include "pipelineTypes.h"
BEGIN_PIPELINE_NAMESPACE

class RenderTarget {
public:
    virtual ~RenderTarget() = default;

    virtual void resize(int width, int height) = 0;

    virtual const Size& getSize() const = 0;
};

END_PIPELINE_NAMESPACE