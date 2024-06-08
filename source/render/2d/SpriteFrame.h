#pragma once

#include "../Texture2D.h"
#include "common/Object.h"
#include "math/math.h"

BEGIN_NS_SCENCE_GRAPH
class SpriteFrame : public Object {
public:
protected:
    Texture2D* m_texture{nullptr};
    Rect m_rect;
};

END_NS_SCENCE_GRAPH