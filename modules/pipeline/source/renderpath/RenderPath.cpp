#pragma once

#include "RenderPath.h"

BEGIN_PIPELINE_NAMESPACE

void RenderPath::addTarget(const RenderTargetDesc& targetDesc) {
    m_renderTargets.emplace_back(targetDesc);
}

void RenderPath::setTarget(size_t index, const RenderTargetDesc& targetDesc) {
    if (index >= m_renderTargets.size()) {
        addTarget(targetDesc);
    } else {
        m_renderTargets[index] = targetDesc;
    }
}

void RenderPath::insertTarget(size_t index, const RenderTargetDesc& targetDesc) {
    if (index >= m_renderTargets.size()) {
        addTarget(targetDesc);
    } else {
        m_renderTargets.insert(m_renderTargets.begin() + index, targetDesc);
    }
}

void RenderPath::removeTarget(const std::string& name) {
    for (auto it = m_renderTargets.begin(); it != m_renderTargets.end(); ++it) {
        if (it->name == name) {
            m_renderTargets.erase(it);
            return;
        }
    }
}

const RenderTargetDesc* RenderPath::getTarget(const std::string& name) const {
    for (auto it = m_renderTargets.begin(); it != m_renderTargets.end(); ++it) {
        if (it->name == name) {
            return &(*it);
        }
    }

    return nullptr;
}

void RenderPath::removeTarget(size_t index) {
    if (index < m_renderTargets.size()) {
        m_renderTargets.erase(m_renderTargets.begin() + index);
    }
}

void RenderPath::addCommand(const RenderCommand& command) {
    m_renderCommands.emplace_back(command);
}

void RenderPath::insertCommand(size_t index, const RenderCommand& command) {
    if (index >= m_renderCommands.size()) {
        addCommand(command);
    } else {
        m_renderCommands.insert(m_renderCommands.begin() + index, command);
    }
}
void RenderPath::setCommand(size_t index, const RenderCommand& command) {
    if (index >= m_renderCommands.size()) {
        addCommand(command);
    } else {
        m_renderCommands[index] = command;
    }
}
void RenderPath::removeCommand(size_t index) {
    if (index < m_renderCommands.size()) {
        m_renderCommands.erase(m_renderCommands.begin() + index);
    }
}

END_PIPELINE_NAMESPACE