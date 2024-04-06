#pragma once

#include <type_traits>

#include "RenderSubpass.h"
#include "RenderTarget.h"
#include "ShaderCode.h"
#include "pipeline_common.h"

BEGIN_PIPELINE_NAMESPACE

struct UniformBinding {
    std::string name;
    gfx::Buffer* buffer{nullptr};
    uint32_t offset{0};
    uint32_t range{0};
};

struct TextureBinding {
    std::string name;
    gfx::Texture* texture{nullptr};
    gfx::Sampler* sampler{nullptr};
};

struct DrawInfo {
    gfx::Viewport viewport;
    gfx::Viewport scissor;
    std::vector<UniformBinding> uniformBindings;
    std::vector<TextureBinding> textureBindings;
    std::vector<gfx::InputAssembler*> inputs;
};

struct DrawInstanceInfo : public DrawInfo {
    uint32_t instanceCount{1};
};

class RenderPipeline {
public:
    RenderPipeline(gfx::Device& device,
                   const std::vector<RenderSubpass*>& subpasses,
                   const std::vector<gfx::Attachment>& attachments,
                   const std::vector<gfx::SubPassDependency>& dependencies)
        : m_device(device),
          m_subpasses(subpasses),
          m_attachments(attachments),
          m_dependencies(dependencies) {}
    virtual ~RenderPipeline();
    void setDrawTarget(RenderTarget* target);

    void begin();
    void setUniform(const UniformBinding& binding);
    void setTexture(const TextureBinding& binding);
    void draw(const DrawInfo& info);
    void drawIntances(const DrawInstanceInfo& info);
    void end();

private:
    gfx::Device& m_device;
    std::vector<gfx::Attachment> m_attachments;
    std::vector<RenderSubpass*> m_subpasses;
    std::vector<gfx::SubPassDependency> m_dependencies;
    gfx::RenderPass* m_renderPass{nullptr};
    RenderTarget* m_target{nullptr};
};

template <class PipelineType>
class RenderPipelineBuilder {
public:
    RenderPipelineBuilder(gfx::Device& device,
                          const std::vector<gfx::Attachment>& attachments)
        : m_device(device), m_attachments(attachments) {}
    RenderSubpass* addSubpass(const std::vector<ShaderCode>& codes,
                              const gfx::PipelineStage& state) {
        gfx::Shader* shader = m_device.createShader();
        for (const auto& code : codes) {
            shader->addStage(code.code, code.stage, code.entryName);
        }

        uint32_t index         = m_subpasses.size();
        RenderSubpass* subpass = new RenderSubpass(index, shader, state);

        return subpass;
    }

    void addDependency(gfx::SubPassDependence src, gfx::SubPassDependence dst,
                       gfx::DependencyFlags flags = {}) {
        m_dependencies.push_back({src, dst, flags});
    }

    RenderPipeline* build() {
        static_assert(std::is_base_of<RenderPipeline, PipelineType>::value);

        PipelineType* pipeline = new PipelineType(
            m_device, m_subpasses, m_attachments, m_dependencies);

        return pipeline;
    }

private:
    gfx::Device& m_device;
    std::vector<gfx::Attachment> m_attachments;
    std::vector<gfx::SubPassDependency> m_dependencies;
    std::vector<RenderSubpass*> m_subpasses;
};

END_PIPELINE_NAMESPACE