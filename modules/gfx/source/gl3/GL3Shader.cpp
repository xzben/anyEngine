#include "GL3Shader.h"

#include "GL3Buffer.h"
#include "GL3Device.h"

BEGIN_GFX_NAMESPACE

static const GLenum g_glStage[] = {
    GL_VERTEX_SHADER,
    GL_GEOMETRY_SHADER,
    GL_FRAGMENT_SHADER,
    GL_COMPUTE_SHADER,
};

static inline OGL_HANDLE createShaderModel(const uint8_t* code, uint32_t size, ShaderStage stage) {
    OGL_HANDLE handle{OGL_NULL_HANDLE};
    GLenum glStage = g_glStage[int(stage)];

    GL_CHECK(handle = glCreateShader(glStage));
    const char* source = (const char*)code;
    GL_CHECK(glShaderSource(handle, 1, (const GLchar**)&source, nullptr));
    GL_CHECK(glCompileShader(handle));
    GLint status;
    GL_CHECK(glGetShaderiv(handle, GL_COMPILE_STATUS, &status));
    if (status != GL_TRUE) {
        char infoLog[1024];
        GL_CHECK(glGetShaderInfoLog(handle, 1024, nullptr, infoLog));
        CCERROR("%s compilation failed.\r\n errorInfo:%s", source, infoLog);
        GL_CHECK(glDeleteShader(handle));
        return OGL_NULL_HANDLE;
    }

    return handle;
}

GL3Shader::GL3Shader(GL3Device& device, ShaderModuleInfo* infos, uint32_t count)
    : m_device(device) {
    m_device.runWithContext([&](gl3::GLContext* ctx) {
        this->createPrograme(infos, count);
        this->reflect();
    });
}

void GL3Shader::createPrograme(ShaderModuleInfo* infos, uint32_t count) {
    GL_CHECK(m_handle = glCreateProgram());

    std::vector<OGL_HANDLE> shaderModules;
    shaderModules.resize(count);
    for (int i = 0; i < count; i++) {
        auto info        = infos[i];
        auto stageHandle = createShaderModel(info.pData, info.size, info.stage);
        shaderModules[i] = stageHandle;
        GL_CHECK(glAttachShader(m_handle, stageHandle));
    }

    GL_CHECK(glLinkProgram(m_handle));
    GLint status;
    GL_CHECK(glGetProgramiv(m_handle, GL_LINK_STATUS, &status));
    if (GL_TRUE != status) {
        CCERROR("Failed to link shader");

        char infoLog[1024];
        glGetProgramInfoLog(m_handle, 1024, NULL, infoLog);
        CCERROR("shader link failed %s", infoLog);
        return;
    }

    for (auto& stageHandle : shaderModules) {
        GL_CHECK(glDeleteShader(stageHandle));
    }
    shaderModules.clear();
}

GL3Shader::~GL3Shader() {
    OGL_HANDLE handle = m_handle;
    m_handle          = OGL_NULL_HANDLE;
    if (handle != OGL_NULL_HANDLE) {
        m_device.runWithContext([=](gl3::GLContext* ctx) { GL_CHECK(glDeleteProgram(handle)); });
    }
}

#define NAME_BUFFER_SIZE 512

void GL3Shader::reflect() {
    // blocks
    GLint blockCount = 0;
    char var_name[NAME_BUFFER_SIZE];
    GLsizei glLength;

    GL_CHECK(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_BLOCKS, &blockCount));

    int32_t data_size, active_uniform;
    int32_t namelength;
    uint32_t indices[100];
    int32_t uniType, uniSize, uniOffset, uniMatStride, uniArrayStride;

    uint32_t binding_index = 0;
    int32_t shader_need;

    for (GLint block_index = 0; block_index < blockCount; ++block_index) {
        GL_CHECK(glGetActiveUniformBlockName(m_handle, block_index, NAME_BUFFER_SIZE - 1, &glLength,
                                             var_name));
        var_name[glLength + 1] = '\0';

        char* offset = strchr(var_name, '[');
        if (offset) {
            var_name[offset - var_name] = '\0';
        }

        BufferBinding binding;
        GL_CHECK(binding.location = glGetUniformBlockIndex(m_handle, var_name));
        binding.name    = var_name;
        binding.binding = binding_index++;

        m_uniformBinding[binding.name] = binding;
    }

    // textures
    int32_t uniform_count;
    GL_CHECK(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &uniform_count));
    uint32_t texture_biding = 0;
    for (uint32_t uniform_index = 0; uniform_index < uniform_count; uniform_index++) {
        GL_CHECK(glGetActiveUniformName(m_handle, uniform_index, NAME_BUFFER_SIZE - 1, &namelength,
                                        var_name));
        GL_CHECK(glGetActiveUniformsiv(m_handle, 1, &uniform_index, GL_UNIFORM_TYPE, &uniType));
        switch (uniType) {
            case GL_SAMPLER_2D:
            case GL_SAMPLER_2D_ARRAY:
            case GL_SAMPLER_3D:
            case GL_SAMPLER_CUBE: {
                TextureBinding binding;
                binding.name    = var_name;
                binding.binding = texture_biding++;
                GL_CHECK(binding.location = glGetUniformBlockIndex(m_handle, var_name));

                m_textureBinding[binding.name] = binding;
            }
        }
    }
}
void GL3Shader::setUniformBuffer(const std::string& name, Buffer* buffer, uint32_t offset,
                                 uint32_t size) {
    auto it = m_uniformBinding.find(name);

    if (it == m_uniformBinding.end()) {
        CCWARN("can't uniform buffer[%s]", name.c_str());
        return;
    }

    it->second.buffer = buffer;
    it->second.offset = offset;
    it->second.size   = size == 0 ? buffer->size() : size;
}

void GL3Shader::setShaderStorageBuffer(const std::string& name, Buffer* buffer, uint32_t offset,
                                       uint32_t size) {
    auto it = m_shaderStorageBinding.find(name);

    if (it == m_shaderStorageBinding.end()) {
        CCWARN("can't shader storage buffer[%s]", name.c_str());
        return;
    }

    it->second.buffer = buffer;
    it->second.offset = offset;
    it->second.size   = size == 0 ? buffer->size() : size;
}
void GL3Shader::setTexture(const std::string& name, Texture* texture, Sampler* sampler) {
    auto it = m_textureBinding.find(name);
    if (it == m_textureBinding.end()) {
        CCWARN("can't find texture binding[%s]", name.c_str());
        return;
    }

    it->second.texture = texture;
    it->second.sampler = sampler;
}

void GL3Shader::bind() {
    GL_CHECK(glUseProgram(m_handle));
    activeBinding();
}
void GL3Shader::activeBinding() {
    for (auto it = m_uniformBinding.begin(); it != m_uniformBinding.end(); it++) {
        const auto& binding = it->second;
        if (binding.buffer != nullptr) {
            OGL_HANDLE bufHandle = binding.buffer->getHandle<OGL_HANDLE>();
            GL_CHECK(glBindBufferRange(GL_UNIFORM_BUFFER, binding.binding, bufHandle,
                                       binding.offset, binding.size));
            GL_CHECK(glUniformBlockBinding(m_handle, binding.location, binding.binding));
        }
    }

    for (auto it = m_shaderStorageBinding.begin(); it != m_shaderStorageBinding.end(); it++) {
        const auto& binding = it->second;
        if (binding.buffer != nullptr) {
            GL_CHECK(glBindBufferRange(GL_SHADER_STORAGE_BUFFER, binding.binding,
                                       binding.buffer->getHandle<OGL_HANDLE>(), binding.offset,
                                       binding.size));
        }
    }

    for (auto it = m_textureBinding.begin(); it != m_textureBinding.end(); it++) {
        const auto& binding = it->second;
        if (binding.texture != nullptr) {
            GL_CHECK(glActiveTexture(GL_TEXTURE0 + binding.binding));
            auto target = GL3Texture::getTarget(binding.texture->getInfo().type);

            GL_CHECK(glBindTexture(target, binding.texture->getHandle<OGL_HANDLE>()));

            if (binding.sampler) {
                GL_CHECK(glBindSampler(GL_TEXTURE0 + binding.binding,
                                       binding.sampler->getHandle<OGL_HANDLE>()));
            }
        }
    }
}

bool GL3Shader::build() { return true; }
END_GFX_NAMESPACE