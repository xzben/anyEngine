#pragma once

#include "../base/RenderPass.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3RenderPass : public RenderPass {
public:
    GL3RenderPass(GL3Device& device, const std::vector<Attachment>& attachments,
                  const std::vector<SubPass>& subpasses,
                  const std::vector<SubPassDependency>& dependencies);
    virtual ~GL3RenderPass();

    const std::vector<Attachment>& getAttachments() { return m_attachments; }
    const std::vector<SubPass>& getSubpasses() { return m_subpasses; }
    const std::vector<SubPassDependency>& getDependencies() {
        return m_dependencies;
    }

private:
    GL3Device& m_device;
    std::vector<Attachment> m_attachments;
    std::vector<SubPass> m_subpasses;
    std::vector<SubPassDependency> m_dependencies;
};

END_GFX_NAMESPACE