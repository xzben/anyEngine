#pragma once

#include <memory>
#include <unordered_map>

#include "RenderPass.h"
#include "pipelineMacro.h"

BEGIN_PIPELINE_NAMESPACE

class RenderPassMgr {
public:
    template <typename T, typename... Args>
    T* addRenderPass(const std::string& name) {
        static_assert(std::is_base_of_v<RenderPass, T>, "T must be derived from RenderPass");
        assert(m_renderPasses.find(name) == m_renderPasses.end());
        m_renderPasses[name] = std::make_unique<T>(name, std::forward<Args>(args)...);
    }

    RenderPass* getRenderPass(const std::string& name) {
        auto it = m_renderPasses.find(name);
        if (it == m_renderPasses.end()) {
            return nullptr;
        }
        return it->second.get();
    }

private:
    std::unordered_map<std::string, std::unique_ptr<RenderPass>> m_renderPasses;
};
END_PIPELINE_NAMESPACE