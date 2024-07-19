#pragma once

#include "FrameGraphConfig.h"
#include "Resource.h"
BEGIN_NS_FG

class FrameGraph;
class RenderTaskBase;
class ResourceBase;

class RenderTaskBuilder {
public:
    explicit RenderTaskBuilder(FrameGraph* fg, RenderTaskBase* task) : m_fg(fg), m_task(task) {}

    RenderTaskBuilder(const RenderTaskBuilder&)            = default;
    RenderTaskBuilder(RenderTaskBuilder&&)                 = default;
    RenderTaskBuilder& operator=(const RenderTaskBuilder&) = default;
    RenderTaskBuilder& operator=(RenderTaskBuilder&&)      = default;

    template <class T, class... ARGS>
    Resource<T>* create(const std::string& name, ARGS... args);

    template <class T>
    Resource<T>* read(Resource<T>* resource);

    template <class T>
    Resource<T>* write(Resource<T>* resource);

    RenderTaskBase* task() const { return m_task; }
    FrameGraph* frameGraph() const { return m_fg; }

private:
    FrameGraph* m_fg;
    RenderTaskBase* m_task;
};

END_NS_FG