#pragma once

#include "common/Object.h"
#include "gfx.h"

BEGIN_NS_SCENCE_GRAPH

class Image;

class Texture2D : public Object {
    DECLARE_RUNTIME_CLASS(Texture2D)
public:
    Texture2D(const char* filename);
    Texture2D(const Image* image);
    virtual ~Texture2D();

private:
    gfx::Texture* m_texture{nullptr};
    const Image* m_image{nullptr};
};

END_NS_SCENCE_GRAPH