#pragma once

#include "../Texture2D.h"
#include "common/Object.h"
#include "math/math.h"

class SpriteFrame : public Object {
public:
protected:
    Texture2D* m_texture{nullptr};
    Rect m_rect;
};