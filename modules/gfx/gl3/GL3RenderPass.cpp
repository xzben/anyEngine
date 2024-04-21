#include "GL3RenderPass.h"

BEGIN_GFX_NAMESPACE
GL3RenderPass::GL3RenderPass(GL3Device& device,
                             const std::vector<Attachment>& attachments,
                             const std::vector<SubPass>& subpass,
                             const std::vector<SubPassDependency>& dependencies)
    : m_device(device) {}
GL3RenderPass::~GL3RenderPass() {}
END_GFX_NAMESPACE