#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Buffer;

class InputAssembler {
public:
    InputAssembler()          = default;
    virtual ~InputAssembler() = default;

    virtual const std::vector<Attribute>& getAttributes() const = 0;
    virtual Buffer* getVertexBuffer() const                     = 0;
    virtual Buffer* getIndexBuffer() const                      = 0;
    virtual uint32_t getVertexCount() const                     = 0;
    virtual uint32_t getIndexCount() const                      = 0;
    virtual uint32_t getVertexItemSize() const                  = 0;
    virtual uint32_t getIndexItemSize() const                   = 0;
    virtual PrimitiveType getPrimivateModel() const            = 0;
};

END_GFX_NAMESPACE