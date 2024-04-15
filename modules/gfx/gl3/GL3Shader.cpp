#include "GL3Shader.h"

BEGIN_GFX_NAMESPACE

GL3Shader::GL3Shader(gl3::GLContext& context) : m_context(context) {}

GL3Shader::~GL3Shader() {}

bool GL3Shader::addStage(const std::vector<uint8_t>& code,
                         gfx::ShaderStage stage, const std::string& entryName) {
    return true;
}
bool GL3Shader::build() { return true; }
END_GFX_NAMESPACE