#pragma once

#include <memory>

#include "FrameGraphConfig.h"

BEGIN_NS_FG

template <class T, class... ARGS>
struct MissingRealizeImplementation : std::false_type {};

// 默认实现，如果没有特化，则返回nullptr，并且产生编译错误
template <class T, class... ARGS>
std::unique_ptr<T> realize(ARGS&&... args) {
    static_assert(MissingRealizeImplementation<T, ARGS...>::value,
                  "Missing realize implementation for description - typepair.");
    return nullptr;
}

/*
 *    资源对象构造模版
 *    实现可以根据 传递的参数是否符合构造 类型的构造函数参数，选择用 构造函数 make_unique 还是 调用
 *    realize 构造 构造函数参数类型不符合时，会产生编译错误
 */
template <class T>
class ResourceRealizer {
public:
    template <typename Tuple>
    std::unique_ptr<T> construct(Tuple&& tuple) {
        return construct(
            std::forward<Tuple>(tuple),
            std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }

    template <typename Tuple>
    std::unique_ptr<T> realize(Tuple&& tuple) {
        return realize(
            std::forward<Tuple>(tuple),
            std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }

private:
    template <class Tuple, std::size_t... I>
    std::unique_ptr<T> construct(Tuple&& tuple, std::index_sequence<I...>) {
        return std::make_unique<T>(std::get<I>(std::forward<Tuple>(tuple))...);
    }

    template <class Tuple, std::size_t... I>
    std::unique_ptr<T> realize(Tuple&& tuple, std::index_sequence<I...>) {
        return NS_FG::realize<T>(std::get<I>(std::forward<Tuple>(tuple))...);
    }
};
END_NS_FG