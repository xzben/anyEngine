#pragma once

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "FrameGraphConfig.h"

BEGIN_NS_FG

class FrameGraph;
class RenderTaskBase;
class RenderTaskBuilder;

class ResourceBase {
    friend class FrameGraph;
    friend class RenderTaskBuilder;

public:
    ResourceBase(const std::string& name, const RenderTaskBase* creator)
        : m_name(name), m_creator(creator) {
        static uint32_t id = 0;
        m_id               = id++;
    }
    virtual ~ResourceBase() = default;

    uint32_t id() const { return m_id; }
    const std::string& name() const { return m_name; }

    // 是否是临时资源
    bool transient() const { return m_creator != nullptr; }

protected:
    virtual void realize()   = 0;
    virtual void derealize() = 0;

protected:
    uint32_t m_id;
    std::string m_name;
    int32_t m_refCount = 0;

    const RenderTaskBase* m_creator;
    std::vector<const RenderTaskBase*> m_readers;
    std::vector<const RenderTaskBase*> m_writers;
};

template <typename T>
class Resource : public ResourceBase {
public:
    template <typename... Args>
    Resource(const std::string& name, const RenderTaskBase* creator, Args&&... args)
        : ResourceBase(name, creator), m_realizer([arg_tuple = std::forward_as_tuple(args...)]() {
              if (constexpr(std::is_constructible_v<T, Args...>)) {
                  return ResourceRealizer<T>().construct(arg_tuple);
              } else {
                  return ResourceRealizer<T>().realize(arg_tuple);
              }
          }) {}
    Resource(const std::string& name, T* actual) : ResourceBase(name, nullptr), m_actual(actual) {}

    Resource(const Resource&)            = delete;
    Resource(Resource&&)                 = default;
    Resource& operator=(const Resource&) = delete;
    Resource& operator=(Resource&&)      = default;

    T* actual() const {
        if (std::holds_alternative<std::unique_ptr<T>>(m_actual)) {
            return std::get<std::unique_ptr<T>>(m_actual).get();
        }

        return std::get<T*>(m_actual);
    }

protected:
    void realize() override {
        if (!transient()) {
            return;
        }

        m_actual = m_realizer();
    }

    void derealize() override {
        if (!transient()) {
            return;
        }
        m_actual = nullptr;
    }

protected:
    std::function<std::unique_ptr<T>()> m_realizer;
    std::variant<std::unique_ptr<T>, T*> m_actual;
};
END_NS_FG