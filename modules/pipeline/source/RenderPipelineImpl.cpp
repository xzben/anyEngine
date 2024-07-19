#include "RenderPipelineImpl.h"

#include "RenderPassMgr.h"
#include "RenderTarget.h"
#include "resources/RenderResources.h"
BEGIN_PIPELINE_NAMESPACE

RenderPipeline* RenderPipeline::create(gfx::Device& device, RenderPassMgr& renderPassMgr) {
    return new RenderPipelineImpl(device, renderPassMgr);
}
RenderPipelineImpl::RenderPipelineImpl(gfx::Device& device, RenderPassMgr& renderPassMgr)
    : m_device(device), m_renderPassMgr(renderPassMgr) {}

RenderPipelineImpl::~RenderPipelineImpl() {}

using FgRenderTexture = NS_FG::Resource<RenderTexture>;

struct RenderPassData {
    std::string passName;
    std::unordered_map<std::string, FgRenderTexture*> resources;
};
using FgRenderTask = NS_FG::RenderTask<RenderPassData>;
struct RenderPipelineData {
    std::vector<FgRenderTask*> renderTask;
    std::unordered_map<std::string, FgRenderTexture*> resources;
    std::vector<std::unique_ptr<RenderTexture>> retainResources;
};

void RenderPipelineImpl::setup(RenderPath* path, RenderTarget* target) {
    Size size = target->getSize();

    m_frameGraph.clear();

    RenderPipelineData pipelineData;
    auto& renderTargets = path->getRenderTargets();
    for (auto& rt : renderTargets) {
        auto res = std::make_unique<RenderTexture>(&m_device, rt, size);
        pipelineData.retainResources.push_back(std::move(res));
        auto fgRes = m_frameGraph.addRetainedResource<RenderTexture>(rt.name, res.get());
        pipelineData.resources[rt.name] = fgRes;
    }

    auto& commands = path->getRenderCommands();

    for (auto& cmd : commands) {
        auto task = m_frameGraph.addTask<RenderPassData>(
            cmd.passName,
            [&pipelineData, cmd, this, path, target](RenderPassData& data,
                                                     NS_FG::RenderTaskBuilder& builder) {
                // setup render pass
                auto renderPass = m_renderPassMgr.getRenderPass(cmd.passName);
                assert(renderPass);

                for (auto& createRes : cmd.innerTargets) {
                    auto fgRes = builder.create<RenderTexture>(createRes.name, &m_device, createRes,
                                                               target->getSize());
                    data.resources[createRes.name] = fgRes;
                }

                for (auto& inputName : cmd.inputs) {
                    auto fgRes = pipelineData.resources[inputName];
                    builder.read(fgRes);
                    data.resources[inputName] = fgRes;
                }

                for (auto& outputName : cmd.outputs) {
                    auto fgRes = data.resources[outputName];
                    if (fgRes == nullptr) {
                        fgRes = pipelineData.resources[outputName];
                    }
                    builder.write(fgRes);
                    data.resources[outputName] = fgRes;
                }
            },
            [](const RenderPassData& data) {
                // execute render pass
            });
        // m_frameGraph.addTask();
    }
}

void RenderPipelineImpl::render(const std::vector<RenderNode*>& nodes, const RenderView& view) {}

END_PIPELINE_NAMESPACE