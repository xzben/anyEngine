#pragma once

#include <cstdint>
#include <string>

#include "common/Object.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
#define MIN_DATA_BUFF 64
using byte = uint8_t;

class Data : public Object {
public:
    Data();
    ~Data();
    void reverse(uint32_t size);
    void reset();

    uint32_t getSize() { return m_size; }
    byte *getData() { return m_data; }

    uint32_t readBuff(byte *dest, uint32_t size);
    uint32_t readOffset(uint32_t offet);

    uint32_t writeBuff(const byte *dest, uint32_t size);
    uint32_t writeOffset(uint32_t offset);

    void resetReadOffset() { m_rIndex = 0; }
    void resetWriteOffset() { m_size = 0; }
    void resetBuffer() {
        resetReadOffset();
        resetWriteOffset();
    }

    std::string getString() { return (char *)getData(); }

private:
    byte *m_data{nullptr};
    uint32_t m_capacity{0};
    uint32_t m_size{0};
    uint32_t m_rIndex{0};
};

END_NS_SCENCE_GRAPH