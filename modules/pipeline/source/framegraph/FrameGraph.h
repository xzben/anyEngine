#pragma once
#include <cassert>
#include <fstream>
#include <memory>
#include <vector>

#include "FrameGraphConfig.h"
#include "RenderTask.h"
#include "RenderTaskBuilder.h"
#include "Resource.h"

BEGIN_NS_FG

class FrameGraph {
    friend class RenderTaskBuilder;

public:
    FrameGraph() = default;

    FrameGraph(const FrameGraph &) = delete;
    FrameGraph(FrameGraph &&)      = default;

    FrameGraph &operator=(const FrameGraph &) = delete;
    FrameGraph &operator=(FrameGraph &&)      = default;

    ~FrameGraph() = default;

    template <class DataType, class... ARGS>
    RenderTask<DataType> *addTask(ARGS &&...args) {
        auto task   = std::make_unique<RenderTask<DataType>>(std::forward<ARGS>(args)...);
        auto *ptask = task.get();
        m_tasks.emplace_back(std::move(task));

        RenderTaskBuilder builder(this, ptask);
        ptask->setup(builder);
        return ptask;
    }

    template <class ActualType>
    Resource<ActualType> *addRetainedResource(const std::string &name,
                                              ActualType *actual = nullptr) {
        auto res   = std::make_unique<Resource<ActualType>>(name, actual);
        auto *pres = res.get();
        m_resources.emplace_back(std::move(res));
        return pres;
    }

    void compile() {
        countRef();
        build();
    }
    void execute() {
        for (auto &step : m_timeline) {
            for (auto *resource : step.realizedResources) {
                resource->realize();
            }

            step.task->execute();

            for (auto *resource : step.derealizedResources) {
                resource->derealize();
            }
        }
    }

    void clear() {
        m_timeline.clear();
        m_tasks.clear();
        m_resources.clear();
    }

    void exportGraphviz(const std::string &filepath) {
        std::ofstream stream(filepath);
        stream << "digraph framegraph \n{\n";

        stream << "rankdir = LR\n";
        stream << "bgcolor = black\n\n";
        stream << "node [shape=rectangle, fontname=\"helvetica\", fontsize=12]\n\n";

        for (auto &task : m_tasks) {
            stream << "\"" << task->name() << "\" [label=\"" << task->name()
                   << "\\nRefs: " << task->m_refCount
                   << "\", style=filled, fillcolor=darkorange]\n";
        }
        stream << "\n";

        for (auto &resource : m_resources) {
            stream << "\"" << resource->name() << "\" [label=\"" << resource->name()
                   << "\\nRefs: " << resource->m_refCount << "\\nID: " << resource->id()
                   << "\", style=filled, fillcolor= "
                   << (resource->transient() ? "skyblue" : "steelblue") << "]\n";
        }
        stream << "\n";

        for (auto &task : m_tasks) {
            stream << "\"" << task->name() << "\" -> { ";
            for (auto &resource : task->m_creates) stream << "\"" << resource->name() << "\" ";
            stream << "} [color=seagreen]\n";

            stream << "\"" << task->name() << "\" -> { ";
            for (auto &resource : task->m_writes) stream << "\"" << resource->name() << "\" ";
            stream << "} [color=gold]\n";
        }
        stream << "\n";

        for (auto &resource : m_resources) {
            stream << "\"" << resource->name() << "\" -> { ";
            for (auto &task : resource->m_readers) stream << "\"" << task->name() << "\" ";
            stream << "} [color=firebrick]\n";
        }
        stream << "}";
    }

private:
    void countRef() {
        for (auto &task : m_tasks) {
            task->m_refCount = task->m_creates.size() + task->m_writes.size();
        }

        for (auto &resource : m_resources) {
            resource->m_refCount = resource->m_readers.size();
        }

        std::vector<ResourceBase *> unrefed_resources;
        for (auto &resource : m_resources) {
            if (resource->m_refCount == 0 && resource->transient())
                unrefed_resources.push_back(resource.get());
        }

        auto unref_task = [&unrefed_resources](RenderTaskBase *task) {
            assert(task->m_refCount > 0);
            task->m_refCount--;

            if (task->m_refCount == 0) {
                for (auto *r : task->m_reads) {
                    assert(r->m_refCount > 0);
                    r->m_refCount--;
                    if (r->m_refCount == 0 && r->transient()) {
                        unrefed_resources.push_back(r);
                    }
                }
            }
        };

        while (!unrefed_resources.empty()) {
            auto *resource = unrefed_resources.back();
            unrefed_resources.pop_back();

            unref_task(const_cast<RenderTaskBase *>(resource->m_creator));

            for (auto *writer : resource->m_writers) {
                unref_task(const_cast<RenderTaskBase *>(writer));
            }
        }
    }

    void build() {
        for (auto itor = m_tasks.begin(); itor != m_tasks.end(); ++itor) {
            auto &task = *itor;
            if (task->m_refCount == 0) {
                return;
            }

            std::vector<ResourceBase *> realized_resources;
            std::vector<ResourceBase *> derealized_resources;

            for (auto resource : task->m_creates) {
                realized_resources.push_back(const_cast<ResourceBase *>(resource));

                if (resource->m_readers.empty() && resource->m_writers.empty()) {
                    derealized_resources.push_back(const_cast<ResourceBase *>(resource));
                }
            }

            auto has_access_after = [&task, &itor, this](const ResourceBase *resource) {
                if (!resource->m_readers.empty()) {
                    auto it =
                        std::find_if(m_tasks.begin(), m_tasks.end(),
                                     [&resource](const std::unique_ptr<RenderTaskBase> &iteratee) {
                                         return iteratee.get() == resource->m_readers.back();
                                     });

                    if (it > itor) {
                        return true;
                    }
                }

                if (!resource->m_writers.empty()) {
                    auto it =
                        std::find_if(m_tasks.begin(), m_tasks.end(),
                                     [&resource](const std::unique_ptr<RenderTaskBase> &iteratee) {
                                         return iteratee.get() == resource->m_writers.back();
                                     });

                    return it > itor;
                }

                return false;
            };

            for (auto resource : task->m_reads) {
                if (!has_access_after(resource)) {
                    derealized_resources.push_back(const_cast<ResourceBase *>(resource));
                }
            }

            for (auto resource : task->m_writes) {
                if (!has_access_after(resource)) {
                    derealized_resources.push_back(const_cast<ResourceBase *>(resource));
                }
            }

            m_timeline.emplace_back(
                Step{task.get(), std::move(realized_resources), std::move(derealized_resources)});
        }
    }

private:
    std::vector<std::unique_ptr<RenderTaskBase>> m_tasks;
    std::vector<std::unique_ptr<ResourceBase>> m_resources;

    struct Step {
        RenderTaskBase *task;
        std::vector<ResourceBase *> realizedResources;
        std::vector<ResourceBase *> derealizedResources;
    };

    std::vector<Step> m_timeline;
};

template <class T, class... ARGS>
Resource<T> *RenderTaskBuilder::create(const std::string &name, ARGS... args) {
    auto res  = std::make_unique<Resource<T>>(name, m_task, std::forward<ARGS>(args)...);
    auto pres = res.get();
    m_fg->m_resources.emplace_back(std::move(res));

    m_task->m_creates.push_back(pres);
    return pres;
}

template <class T>
Resource<T> *RenderTaskBuilder::read(Resource<T> *resource) {
    m_task->m_reads.push_back(resource);
    resource->m_readers.push_back(m_task);
    return resource;
}

template <class T>
Resource<T> *RenderTaskBuilder::write(Resource<T> *resource) {
    m_task->m_writes.push_back(resource);
    resource->m_writers.push_back(m_task);
    return resource;
}

END_NS_FG