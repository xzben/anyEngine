#pragma once

#include <type_traits>

#include "RenderSubpass.h"
#include "RenderTarget.h"
#include "ShaderCode.h"
#include "pipeline_common.h"

BEGIN_PIPELINE_NAMESPACE

struct BufferBinding {
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
    std::vector<BufferBinding> uniformBindings;
    std::vector<TextureBinding> textureBindings;
    std::vector<gfx::InputAssembler*> inputs;
};

struct DrawInstanceInfo : public DrawInfo {
    uint32_t instanceCount{1};
};

class RenderPipeline {
public:
    RenderPipeline(gfx::Device& device, gfx::RenderPass* renderPass,
                   const std::vector<RenderSubpass*>& subpasses);
    virtual ~RenderPipeline();
    void setDrawTarget(RenderTarget* target);

    void begin();
    void setUniform(const BufferBinding& binding);
    void setTexture(const TextureBinding& binding);
    void draw(const DrawInfo& info);
    void drawIntances(const DrawInstanceInfo& info);
    void end();

private:
    gfx::Device& m_device;
    gfx::RenderPass* m_renderPass{nullptr};
    std::vector<RenderSubpass*> m_subpasses;
    RenderTarget* m_target{nullptr};
};

template <class PipelineType>
class RenderPipelineBuilder {
public:
    RenderPipelineBuilder(gfx::Device& device,
                          const std::vector<gfx::Attachment>& attachments)
        : m_device(device), m_attachments(attachments) {}
    RenderSubpass* addSubpass(const std::vector<ShaderCode>& codes,
                              const gfx::PipelineState& state) {
        
        std::vector<gfx::ShaderModuleInfo> infos;
        infos.resize(codes.size());
        
        for(int i = 0; i < codes.size(); i++)
        {
            auto& code = codes[i];
            auto& info = infos[i];
            info.pData = code.code.data();
            info.size = code.code.size();
            info.stage = code.stage;
            info.entryName = code.entryName;
        }
        gfx::Shader* shader = m_device.createShader(infos.data(), infos.size());
        
        uint32_t index         = m_subpasses.size();
        RenderSubpass* subpass = new RenderSubpass(m_device, index, shader, state);

        return subpass;
    }

    void addDependency(gfx::SubPassDependence src, gfx::SubPassDependence dst,
                       gfx::DependencyFlags flags = {}) {
        m_dependencies.push_back({src, dst, flags});
    }

    RenderPipeline* build() {
        static_assert(std::is_base_of<RenderPipeline, PipelineType>::value);

        std::vector<gfx::SubPass> subpassInfos;
        int count = m_subpasses.size();
        subpassInfos.resize(count);
        for (int i = 0; i < count; i++) {
            subpassInfos[i] = m_subpasses[i]->getSubpassInfo();
        }
        gfx::RenderPass* renderPass = m_device.createRenderPass(
            m_attachments, subpassInfos, m_dependencies);

        for (int i = 0; i < count; i++) {
            m_subpasses[i]->build(renderPass);
        }
        PipelineType* pipeline =
            new PipelineType(m_device, renderPass, m_subpasses);

        return pipeline;
    }

private:
    gfx::Device& m_device;
    std::vector<gfx::Attachment> m_attachments;
    std::vector<gfx::SubPassDependency> m_dependencies;
    std::vector<RenderSubpass*> m_subpasses;
    std::vector<gfx::SubPass> m_subpassInfos;
};

END_PIPELINE_NAMESPACE
