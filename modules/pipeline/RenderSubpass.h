#include "NoCopy.h"
#include "pipeline_common.h"
BEGIN_PIPELINE_NAMESPACE

class RenderSubpass final : public NoCopy {
public:
    RenderSubpass(gfx::Device& device, uint32_t index, gfx::Shader* shader,
                  const gfx::PipelineState& state);
    virtual ~RenderSubpass();
    void addInput(uint32_t attachment, gfx::ImageLayout layout);
    void addColor(uint32_t attachment, gfx::ImageLayout layout);
    void addDepthStencil(uint32_t attachment, gfx::ImageLayout layout);
    void addResolve(uint32_t attachment, gfx::ImageLayout layout);
    void addPerserve(uint32_t attachment);

    const gfx::SubPass& getSubpassInfo() { return m_subpassInfo; }

    bool build(gfx::RenderPass* renderPass);

private:
    gfx::Device& m_device;
    gfx::Shader* m_shader{nullptr};
    gfx::PipelineState m_state;
    gfx::SubPass m_subpassInfo;
    uint32_t m_index{0};
    gfx::Pipeline* m_pipeline{nullptr};
};

END_PIPELINE_NAMESPACE