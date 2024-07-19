#pragma once

#include <functional>
#include <string>
#include <vector>

#include "FrameGraphConfig.h"

BEGIN_NS_FG

class FrameGraph;
class ResourceBase;
class RenderTaskBuilder;

class RenderTaskBase {
    friend class RenderTaskBuilder;
    friend class FrameGraph;

public:
    RenderTaskBase(const std::string &name) : m_name(name) {}
    virtual ~RenderTaskBase() = default;

    const std::string &name() const { return m_name; }

    virtual void setup(RenderTaskBuilder &builder) = 0;
    virtual void execute()                         = 0;

private:
    std::string m_name;
    size_t m_refCount = 0;

    std::vector<ResourceBase *> m_creates;
    std::vector<ResourceBase *> m_reads;
    std::vector<ResourceBase *> m_writes;
};

template <class DataType>
class RenderTask : public RenderTaskBase {
public:
    using Setup   = std::function<void(DataType &, RenderTaskBuilder &)>;
    using Execute = std::function<void(const DataType &)>;

public:
    RenderTask(const std::string &name, Setup setup, Execute execute)
        : RenderTaskBase(name), m_setup(std::move(setup)), m_execute(std::move(execute)) {}

    RenderTask(const RenderTask &) = delete;
    RenderTask(RenderTask &&)      = default;

    RenderTask &operator=(const RenderTask &) = delete;
    RenderTask &operator=(RenderTask &&)      = default;

    ~RenderTask() = default;

    const DataType &data() const { return m_data; }

    void setup(RenderTaskBuilder &builder) override { m_setup(m_data, builder); }
    void execute() override { m_execute(m_data); }

private:
    DataType m_data;

    const std::function<void(DataType &, RenderTaskBuilder &)> m_setup;
    const std::function<void(const DataType &)> m_execute;
};

END_NS_FG