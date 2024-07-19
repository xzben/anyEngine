#include "ByteArray.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

BEGIN_PIPELINE_NAMESPACE

ByteArray::ByteArray() {}

ByteArray::~ByteArray() {}

void ByteArray::resize(size_t size) {
    reserve(size);
    m_size = size;
}

void ByteArray::clear() { m_size = 0; }

void ByteArray::append(const void* data, size_t size) {
    reserve(m_size + size);
    memcpy(m_data + m_size, data, size);
    m_size += size;
}

void ByteArray::remove(size_t pos, size_t size) {
    if (pos + size > m_size) {
        size = m_size - pos;
    }

    if (size > 0) {
        memmove(m_data + pos, m_data + pos + size, m_size - pos - size);
        m_size -= size;
    }
}

void ByteArray::reserve(size_t capacity) {
    if (capacity > m_capacity) {
        auto new_capacity = m_capacity;
        while (new_capacity < capacity) {
            new_capacity *= 2;
        }
        m_data     = (uint8_t*)realloc(m_data, new_capacity);
        m_capacity = new_capacity;
    }
}

END_PIPELINE_NAMESPACE