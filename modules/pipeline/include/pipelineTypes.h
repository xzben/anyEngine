#pragma once

#include <string>
#include <variant>
#include <vector>

#include "ByteArray.h"
#include "gfx.h"
#include "glm/glm.hpp"
#include "pipelineMacro.h"

BEGIN_PIPELINE_NAMESPACE

enum class RenderTargetSizeMode { RELATIVE = 0, ABSOLUTE };

struct Size {
    float width{0.0f};
    float height{0.0f};
};

struct RenderTargetDesc {
    std::string name;
    gfx::PixelFormat format;
    Size size;
    RenderTargetSizeMode sizeMode;
};

struct ShadeModule {
    gfx::ShaderStage stage;
    ByteArray code;
};

using ShaderVariant = std::string;
struct Shader {
    std::vector<ShaderVariant> variants;  // 变体名称
    std::string name;
    std::vector<ShadeModule> modules;
};

struct Subpass {
    std::string name;
    Shader shader;
};

struct Pass {
    std::string name;
    std::vector<Subpass> subpasses;
};

using UniformValue =
    std::variant<int32_t, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, gfx::Texture*>;

struct Uniform {
    std::string name;
    UniformValue value;
};

struct Material {
    std::string name;

    std::vector<Uniform> uniforms;
    std::vector<Pass> passes;
};

struct Mesh {
    ByteArray vertexData;
    ByteArray indexData;
    std::vector<gfx::Attribute> attributes;
    gfx::PrimitiveType primitiveType{gfx::PrimitiveType::TRIANGLE_LIST};
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

struct Model {
    std::vector<Mesh*> meshes;
    std::vector<Material*> materials;
    AABB aabb;
};

struct RenderView {
    uint32_t visibleMask{0xFFFFFFFF};  // 可见性掩码
    glm::mat4 view;
    glm::mat4 projection;
};

struct RenderNode {
    uint32_t id{0};
    uint32_t visibleMask{0xFFFFFFFF};  // 可见性掩码
    glm::mat4 transform;
    Model* model{nullptr};
};

END_PIPELINE_NAMESPACE