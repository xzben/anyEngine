#pragma once

#include "../base/Shader.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Shader : public Shader {
public:
    GL3Shader(gl3::GLContext& context);
    virtual ~GL3Shader();

    virtual bool addStage(const std::vector<uint8_t>& code,
                          gfx::ShaderStage stage,
                          const std::string& entryName = "main") override;
    virtual bool build() override;

private:
    gl3::GLContext& m_context;
};

END_GFX_NAMESPACE