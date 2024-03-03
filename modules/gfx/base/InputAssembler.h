#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Buffer;

class InputAssembler {
public:
    virtual Buffer* getVertexBuffer()    = 0;
    virtual Buffer* getIndexBuffer()     = 0;
    virtual uint32_t getVertexCount()    = 0;
    virtual uint32_t getIndexCount()     = 0;
    virtual uint32_t getVertexItemSize() = 0;
    virtual uint32_t getIndexItemSize()  = 0;
};

END_GFX_NAMESPACE