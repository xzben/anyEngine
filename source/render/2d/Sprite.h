#pragma once

#include "app/components/Renderable.h"
#include "math/math.h"

BEGIN_NS_SCENCE_GRAPH

class Texture2D;
class Sprite : public Renderable {
    DECLARE_RUNTIME_CLASS(Sprite)
public:
    Sprite();
    virtual ~Sprite();

    void initWithFile(const std::string& filename);
    void setSize(const Size& size);
    const Size& getSize() { return m_size; }

protected:
    Texture2D* m_texture{nullptr};
    Size m_size{0.f, 0.f};
};

END_NS_SCENCE_GRAPH