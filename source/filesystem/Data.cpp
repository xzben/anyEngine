#include "Data.h"

#include <cstdio>

Data::Data() { reverse(MIN_DATA_BUFF); }

Data::~Data() {
    if (m_data != nullptr) {
        free(m_data);
        m_data = nullptr;
    }
}

void Data::reset() {
    m_size   = 0;
    m_rIndex = 0;
}

void Data::reverse(uint32_t size) {
    if (size <= m_capacity) return;

    if (m_capacity <= 0) m_capacity = 1;
    int oldsize = m_capacity;
    while (m_capacity < size) m_capacity *= 2;

    byte *newdata = (byte *)malloc(m_capacity);
    if (oldsize > 0 && m_data) {
        memset(newdata, 0, m_capacity);
        memcpy(newdata, m_data, oldsize);
    }

    if (m_data != nullptr) {
        free(m_data);
        m_data = nullptr;
    }
    m_data = newdata;
}

uint32_t Data::readBuff(byte *dest, uint32_t size) {
    int rsize      = size;
    int remainSize = m_size - m_rIndex;
    if (rsize > remainSize) {
        rsize = remainSize;
    }

    memcpy(dest, m_data + m_rIndex, rsize);

    return rsize;
}

uint32_t Data::readOffset(uint32_t offet) {
    if (m_rIndex + offet > m_size) offet = m_size - m_rIndex;

    m_rIndex += offet;

    return offet;
}

uint32_t Data::writeBuff(const byte *src, uint32_t size) {
    int remainSize = m_capacity - m_size;

    if (size > remainSize) {
        int newCapcity = m_capacity;
        while (newCapcity - m_size < size) newCapcity *= 2;
        reverse(newCapcity);
    }

    memcpy(m_data + m_size, src, size);
    m_size += size;

    return m_size;
}

uint32_t Data::writeOffset(uint32_t offset) {
    if (m_capacity - m_size < offset) {
        int newCapcity = m_capacity;
        while (newCapcity - m_size < offset) newCapcity *= 2;
        reverse(newCapcity);
    }

    m_size += offset;

    return m_size;
}