#pragma once

#include "Asset.h"
#include "gfx.h"

BEGIN_NS_SCENCE_GRAPH

class Image : public Asset {
public:
    Image();
    virtual ~Image();

    bool initWithImageData(void* imgData, uint32_t dataSize);
    bool initWithPixelData(void* pixelData, uint32_t dataSize, gfx::PixelFormat format,
                           uint32_t width, uint32_t height);

protected:
    virtual bool onLoaded(Data& data) override;
    void freePixelData();

protected:
    uint32_t m_width;
    uint32_t m_height;
    gfx::PixelFormat m_format;
    struct PixelData {
        void* stbPixels{nullptr};
        void* pixelData{nullptr};
    } m_pixelData;
};
END_NS_SCENCE_GRAPH