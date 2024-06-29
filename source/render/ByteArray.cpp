#include "ByteArray.h"

#include <memory>

BEGIN_NS_SCENCE_GRAPH

ByteArray::ByteArray() {}

ByteArray::~ByteArray() { clear(); }

void ByteArray::clear() {
    if (m_data) {
        free(m_data);
        m_data = nullptr;
    }
    m_capacity = 0;
    m_size     = 0;
}

ByteArray::ByteArray(const ByteArray& other) { this->operator=(other); }
ByteArray::ByteArray(ByteArray&& other) { this->operator=(std::move(other)); }

ByteArray& ByteArray::operator=(const ByteArray& other) {
    m_size = 0;
    write(other.m_data, other.m_size);
    return *this;
}

ByteArray& ByteArray::operator=(ByteArray&& other) {
    clear();
    m_data           = other.m_data;
    m_capacity       = other.m_capacity;
    m_size           = other.m_size;
    other.m_data     = nullptr;
    other.m_capacity = 0;
    other.m_size     = 0;
    return *this;
}

uint32_t ByteArray::write(const void* data, uint32_t size) {
    if (m_capacity < size + m_size) {
        reverse(m_size + size);
    }

    memcpy(m_data + m_size, data, size);
    m_size += size;

    return m_size;
}

void ByteArray::reverse(uint32_t size) {
    if (m_capacity >= size) return;

    uint32_t newCapacity = m_capacity * 2;
    while (newCapacity < size) {
        newCapacity *= 2;
    }

    m_data     = (uint8_t*)realloc(m_data, newCapacity);
    m_capacity = newCapacity;
}

END_NS_SCENCE_GRAPH