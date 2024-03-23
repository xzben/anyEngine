#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class RenderSubpass {
public:
    RenderSubpass()          = default;
    virtual ~RenderSubpass() = default;

    virtual void addInput(uint32_t attachment, ImageLayout layout)        = 0;
    virtual void addColor(uint32_t attachment, ImageLayout layout)        = 0;
    virtual void addResolve(uint32_t attachment, ImageLayout layout)      = 0;
    virtual void addDepthStencil(uint32_t attachment, ImageLayout layout) = 0;
    virtual void addPerserve(uint32_t attachment)                         = 0;
};

class RenderPass {
public:
    RenderPass()          = default;
    virtual ~RenderPass() = default;
};

class RenderPassBuilder {
public:
    RenderPassBuilder(const std::vector<Attachment>& attachments)
        : m_attachments(attachments) {}
    void addDependency(SubPassDependence src, SubPassDependence dest,
                       DependencyFlags flags) {
        m_dependencies.push_back({src, dest, flags});
    }

    virtual RenderSubpass* addSubpass() = 0;
    virtual RenderPass* build()         = 0;

protected:
    std::vector<Attachment> m_attachments;
    std::vector<SubPassDependency> m_dependencies;
};

END_GFX_NAMESPACE