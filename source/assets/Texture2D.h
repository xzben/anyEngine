#pragma once

#include "Asset.h"
#include "common/Object.h"
#include "gfx.h"

BEGIN_NS_SCENCE_GRAPH

template <class T>
class ObjectContainor;

class Image;

class Texture2D : public Asset {
    DECLARE_RUNTIME_CLASS(Texture2D)
private:
    friend class AssetMgr;
    friend class ObjectContainor<Asset>;
    Texture2D();

public:
    Texture2D(const char* filename);
    Texture2D(Image* image);
    virtual ~Texture2D();

public:
    bool load(const std::string& path) override;

    uint32_t width() const;
    uint32_t height() const;

protected:
    bool initWithImage(Image* img);
    void freeImage();

private:
    gfx::Texture* m_texture{nullptr};
    Image* m_image{nullptr};
};

END_NS_SCENCE_GRAPH