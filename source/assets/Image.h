#pragma once

#include "Asset.h"
#include "gfx.h"

BEGIN_NS_SCENCE_GRAPH

class Image : public Asset {
    DECLARE_RUNTIME_CLASS(Image)
public:
    Image();
    virtual ~Image();

    bool initWithImageData(void* imgData, uint32_t dataSize);
    bool initWithPixelData(void* pixelData, uint32_t dataSize, NS_GFX::PixelFormat format,
                           uint32_t width, uint32_t height);

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    void* pixels() const {
        if (m_pixelData.stbPixels) {
            return m_pixelData.stbPixels;
        } else if (m_pixelData.pixelData) {
            return m_pixelData.pixelData;
        }

        return nullptr;
    }

    NS_GFX::PixelFormat pixelFormat() const { return m_format; }

protected:
    virtual bool onLoaded(Data& data) override;
    void freePixelData();

protected:
    uint32_t m_width;
    uint32_t m_height;
    NS_GFX::PixelFormat m_format;
    struct PixelData {
        void* stbPixels{nullptr};
        void* pixelData{nullptr};
    } m_pixelData;
};
END_NS_SCENCE_GRAPH