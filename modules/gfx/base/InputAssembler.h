#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Buffer;

class InputAssembler : public GfxObject {
public:
    virtual const std::vector<Attribute>& getAttributes() const = 0;
    virtual Buffer* getVertexBuffer() const                     = 0;
    virtual Buffer* getIndexBuffer() const                      = 0;
    virtual Buffer* getInstanceBuffer() const                   = 0;
    virtual uint32_t getVertexCount() const                     = 0;
    virtual uint32_t getIndexCount() const                      = 0;
    virtual uint32_t getVertexItemSize() const                  = 0;
    virtual uint32_t getIndexItemSize() const                   = 0;

    virtual void updateVertexData(const void* pData, uint32_t size,
                                  uint32_t vertexCount)     = 0;
    virtual void updateIndexData(const void* pData, uint32_t size,
                                 uint32_t indexCount)       = 0;
    virtual void updateInstanceData(const void* pData, uint32_t size,
                                    uint32_t instanceCount) = 0;

protected:
    InputAssembler() : GfxObject(GfxObjectType::InputAssembler) {}
    virtual ~InputAssembler() = default;
};

END_GFX_NAMESPACE