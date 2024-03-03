#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Buffer {
public:
    Buffer(BufferUsage& usage, uint32_t size)
        : m_usage(usage), m_capacity(size) {}
    virtual ~Buffer()                        = default;
    virtual void update(const void* data, uint32_t size,
                        uint32_t offset = 0) = 0;

protected:
    BufferUsage m_usage{BufferUsage::NONE};
    uint32_t m_size{0};
    uint32_t m_capacity{0};
};

END_GFX_NAMESPACE