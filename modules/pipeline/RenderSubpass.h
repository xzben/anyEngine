#include "NoCopy.h"
#include "pipeline_common.h"
BEGIN_PIPELINE_NAMESPACE

class RenderSubpass final : public NoCopy {
public:
    RenderSubpass(uint32_t index, gfx::Shader* shader,
                  const gfx::PipelineState& state);

    void addInput(uint32_t attachment, gfx::ImageLayout layout);
    void addColor(uint32_t attachment, gfx::ImageLayout layout);
    void addDepthStencil(uint32_t attachment, gfx::ImageLayout layout);
    void addResolve(uint32_t attachment, gfx::ImageLayout layout);
    void addPerserve(uint32_t attachment);

private:
    gfx::Shader* m_shader;
    gfx::PipelineState m_state;
    std::vector<gfx::AttachmentReference> m_color_attachments;
    std::vector<gfx::AttachmentReference> m_input_attachments;
    std::vector<gfx::AttachmentReference> m_resolve_attachments;
    std::vector<gfx::AttachmentReference> m_depth_stencil_attachment;
    std::vector<uint32_t> m_preserve_attachments;
    uint32_t m_index{0};
};

END_PIPELINE_NAMESPACE