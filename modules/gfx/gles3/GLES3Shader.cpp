#include "GLES3Shader.h"

BEGIN_GFX_NAMESPACE

GLES3Shader::GLES3Shader(gles::GLContext& context)
    : GfxObject(GfxObjectType::Shader), m_context(context) {}

GLES3Shader::~GLES3Shader() {}

bool GLES3Shader::addStage(const std::vector<uint8_t>& code,
                           gfx::ShaderStage stage,
                           const std::string& entryName) {
    return true;
}
bool GLES3Shader::build() { return true; }
END_GFX_NAMESPACE