#pragma once

#include "../gl3_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

struct BufferBinding {
    std::string name;
    Buffer* buffer;
    uint32_t offset;
    uint32_t size;
};

struct TextureBinding {
    std::string name;
    Texture* texture{nullptr};
    Sampler* sampler{nullptr};
};

struct SubPassDrawInfo {
    std::vecotr<BufferBinding> uniformBuffers;
    std::vecotr<BufferBinding> storageBuffers;
    std::vecotr<TextureBinding> SamplerTextures;
    Pipeline* pipeline{nullptr};
};

struct RenderPassInfo {
    Viewport viewport{0, 0, 0, 0};
    bool enableScissor{false};
    Viewport scissor{0, 0, 0, 0};

    std::vecotr<SubPassDrawInfo> subpasses;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE