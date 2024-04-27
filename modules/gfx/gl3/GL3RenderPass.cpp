#include "GL3RenderPass.h"

BEGIN_GFX_NAMESPACE
GL3RenderPass::GL3RenderPass(GL3Device& device,
                             const std::vector<Attachment>& attachments,
                             const std::vector<SubPass>& subpasses,
                             const std::vector<SubPassDependency>& dependencies)
    : m_device(device),
      m_attachments(attachments),
      m_subpasses(subpasses),
      m_dependencies(dependencies) {}
GL3RenderPass::~GL3RenderPass() {}
END_GFX_NAMESPACE