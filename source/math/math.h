#pragma once
#include "common.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

template <typename T>
class TSize {
public:
    TSize() {}
    TSize(const TSize &rhs) : TSize(rhs.width, rhs.height) {}
    TSize(T w, T h) : width(w), height(h) {}

    TSize &operator=(const TSize &rhs) {
        this->width  = rhs.width;
        this->height = rhs.height;

        return *this;
    }

    T width{0};
    T height{0};
};

template <typename T>
class TRect {
public:
    TRect() {}
    TRect(const TRect &rhs) : TRect(rhs.left, rhs.top, rhs.width, rhs.height) {}
    TRect(T l, T t, T w, T h) : left(l), top(t), width(w), height(h) {}

    TRect &operator=(const TRect &rhs) {
        this->left   = rhs.left;
        this->top    = rhs.top;
        this->width  = rhs.width;
        this->height = rhs.height;
    }

    T left{0};
    T top{0};
    T width{0};
    T height{0};
};

using Size  = TSize<float>;
using Rect  = TRect<float>;
using ISize = TSize<int32_t>;
using IRect = TRect<int32_t>;
using FSize = TSize<float>;
using FRect = TRect<float>;

END_NS_SCENCE_GRAPH