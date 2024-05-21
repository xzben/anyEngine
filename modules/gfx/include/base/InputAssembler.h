#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Buffer;

class InputAssembler : public GfxObject {
public:
    virtual PrimitiveType getPrimitiveType()                            = 0;
    virtual const std::vector<Attribute>& getAttributes() const         = 0;
    virtual const std::vector<Attribute>& getInstanceAttributes() const = 0;

    virtual Buffer* getVertexBuffer() const            = 0;
    virtual Buffer* getIndexBuffer() const             = 0;
    virtual Buffer* getInstanceBuffer() const          = 0;
    virtual uint32_t getVertexCount() const            = 0;
    virtual uint32_t getIndexCount() const             = 0;
    virtual uint32_t getVertexAttribteStride() const   = 0;
    virtual uint32_t getInstanceAttribteStride() const = 0;
    virtual uint32_t getIndexItemSize() const          = 0;

protected:
    InputAssembler() : GfxObject(GfxObjectType::InputAssembler) {}
    virtual ~InputAssembler() = default;
};

END_GFX_NAMESPACE