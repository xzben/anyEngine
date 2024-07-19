#pragma once

#include <cstdint>

#include "pipelineMacro.h"
BEGIN_PIPELINE_NAMESPACE

class ByteArray {
public:
    ByteArray();
    ~ByteArray();

    void resize(size_t size);
    void clear();
    void append(const void* data, size_t size);
    void remove(size_t pos, size_t size);

    void reserve(size_t capacity);
    const void* data() const { return m_data; }
    size_t size() const { return m_size; }

    void* data() { return m_data; }

private:
    uint8_t* m_data{nullptr};
    size_t m_capacity{0};
    size_t m_size{0};
};
END_PIPELINE_NAMESPACE