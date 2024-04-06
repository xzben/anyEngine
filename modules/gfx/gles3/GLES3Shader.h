#pragma once

#include "../base/Shader.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Shader : public Shader, public GfxObject {
public:
    GLES3Shader(gles::GLContext& context);
    virtual ~GLES3Shader();

    virtual bool addStage(const std::vector<uint8_t>& code,
                          gfx::ShaderStage stage,
                          const std::string& entryName = "main") override;
    virtual bool build() override;

private:
    gles::GLContext& m_context;
};

END_GFX_NAMESPACE