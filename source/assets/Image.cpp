#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <memory>

BEGIN_NS_SCENCE_GRAPH
Image::Image() : Asset(AssetType::Image) {}
Image::~Image() { freePixelData(); }

void Image::freePixelData() {
    if (m_pixelData.stbPixels) {
        stbi_image_free(m_pixelData.stbPixels);
    }

    if (m_pixelData.pixelData) {
        free(m_pixelData.pixelData);
    }
    m_pixelData.stbPixels = nullptr;
    m_pixelData.pixelData = nullptr;
}

bool Image::initWithImageData(void* imgData, uint32_t dataSize) {
    freePixelData();

    int channel;
    int width, height;
    m_pixelData.stbPixels = stbi_load_from_memory((unsigned char*)imgData, dataSize, &width,
                                                  &height, &channel, STBI_rgb_alpha);

    m_width  = width;
    m_height = height;
    m_format = NS_GFX::PixelFormat::RGBA8;

    return true;
}

bool Image::initWithPixelData(void* pixelData, uint32_t dataSize, NS_GFX::PixelFormat format,
                              uint32_t width, uint32_t height) {
    freePixelData();
    m_width               = width;
    m_height              = height;
    m_format              = format;
    m_pixelData.pixelData = malloc(dataSize);
    memcpy(m_pixelData.pixelData, pixelData, dataSize);
    return true;
}

bool Image::onLoaded(Data& data) { return initWithImageData(data.getData(), data.getSize()); }

END_NS_SCENCE_GRAPH