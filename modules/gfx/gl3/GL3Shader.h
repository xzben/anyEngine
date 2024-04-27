#pragma once

#include <unordered_map>

#include "../base/Shader.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class Buffer;
class Texture;
class Sampler;

struct BufferBinding {
    std::string name;
    uint32_t binding;
    uint32_t location;

    Buffer* buffer{nullptr};
    uint32_t offset{0};
    uint32_t size{0};
};

struct TextureBinding {
    std::string name;
    uint32_t binding;
    uint32_t location;

    Texture* texture{nullptr};
    Sampler* sampler{nullptr};
};

class GL3Shader : public Shader {
public:
    GL3Shader(GL3Device& device, ShaderModuleInfo* info, uint32_t count);
    virtual ~GL3Shader();

    virtual bool build() override;

    void setUniformBuffer(const std::string& name, Buffer* buffer,
                          uint32_t offset = 0, uint32_t size = 0);
    void setShaderStorageBuffer(const std::string& name, Buffer* buffer,
                                uint32_t offset = 0, uint32_t size = 0);
    void setTexture(const std::string& name, Texture* texture,
                    Sampler* sampler = nullptr);

    void bind();

protected:
    void activeBinding();
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }
    void createPrograme(ShaderModuleInfo* infos, uint32_t count);
    void reflect();

private:
    GL3Device& m_device;
    OGL_HANDLE m_handle{OGL_NULL_HANDLE};

    std::unordered_map<std::string, BufferBinding> m_uniformBinding;
    std::unordered_map<std::string, BufferBinding> m_shaderStorageBinding;
    std::unordered_map<std::string, TextureBinding> m_textureBinding;
};

END_GFX_NAMESPACE