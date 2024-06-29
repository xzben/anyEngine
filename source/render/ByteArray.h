#pragma once
#include <cstdint>

#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

class ByteArray {
public:
    ByteArray();
    ~ByteArray();

    ByteArray(const ByteArray& other);
    ByteArray& operator=(const ByteArray& other);

    ByteArray(ByteArray&& other);
    ByteArray& operator=(ByteArray&& other);
    

    void* data() const { return m_data; }
    uint32_t size() const { return m_size; }
    uint32_t write(const void* data, uint32_t size);
    void reverse(uint32_t size);
protected:
    void clear();
protected:
    uint8_t* m_data{nullptr};
    uint32_t m_capacity{0};
    uint32_t m_size{0};
};

END_NS_SCENCE_GRAPH