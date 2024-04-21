#include "GL3InputAssembler.h"

#include "GL3Device.h"

BEGIN_GFX_NAMESPACE

static const uint32_t g_dataFmtSize[] = {
    0,   // DataFormat::UNDEFINED
    4,   // DataFormat::Bool
    8,   // DataFormat::Bool2
    12,  // DataFormat::Bool3
    16,  // DataFormat::Bool4
    4,   // DataFormat::Int
    8,   // DataFormat::Int2
    12,  // DataFormat::Int3
    16,  // DataFormat::Int4
    4,   // DataFormat::UInt
    8,   // DataFormat::UInt2
    12,  // DataFormat::UInt3
    16,  // DataFormat::UInt4
    4,   // DataFormat::Float
    8,   // DataFormat::Float2
    12,  // DataFormat::Float3
    16,  // DataFormat::Float4
    64,  // DataFormat::Mat4
    36,  // DataFormat::Mat3
};

GL3InputAssembler::GL3InputAssembler(
    GL3Device& device, const std::vector<Attribute>& attributes,
    const void* pVertexData, uint32_t vertexCount, const void* pIndexData,
    uint32_t indexCount, uint32_t indexItemSize)
    : m_device(device),
      m_attributes(attributes),
      m_vertexCount(vertexCount),
      m_indexCount(indexCount),
      m_indexSize(indexItemSize) {
    m_vertexStripe = 0;
    for (auto& attr : m_attributes) {
        uint32_t itemSize = g_dataFmtSize[(int)attr.format];
        m_vertexStripe += itemSize;
    }
    m_vertexBuffer = new GL3Buffer(m_device, BufferType::VERTEX,
                                   vertexCount * m_vertexStripe, pVertexData);

    if (pIndexData != nullptr && indexCount > 0) {
        m_indexBuffer = new GL3Buffer(m_device, BufferType::INDEX,
                                      indexItemSize * indexCount, pIndexData);
    }
}

GL3InputAssembler::GL3InputAssembler(
    GL3Device& device, const std::vector<Attribute>& attributes,
    const std::vector<Attribute>& InstanceAttributes, const void* pVertexData,
    uint32_t vertexCount, const void* pInstanceData, uint32_t instanceCount,
    const void* pIndexData, uint32_t indexCount, uint32_t indexItemSize)
    : GL3InputAssembler(device, attributes, pVertexData, vertexCount,
                        pIndexData, indexCount, indexItemSize) {
    m_instanceAttributes = InstanceAttributes;
    m_instanceCount      = instanceCount;

    m_instanceStrip = 0;
    for (auto& attr : m_instanceAttributes) {
        uint32_t itemSize = g_dataFmtSize[(int)attr.format];
        m_instanceStrip += itemSize;
    }

    m_instanceBuffer =
        new GL3Buffer(m_device, BufferType::VERTEX,
                      vertexCount * m_vertexStripe, pInstanceData);
}

GL3InputAssembler::~GL3InputAssembler() {
    if (m_vertexBuffer) {
        m_device.destroyBuffer(m_vertexBuffer);
        m_vertexBuffer = nullptr;
    }

    if (m_indexBuffer) {
        m_device.destroyBuffer(m_indexBuffer);
        m_indexBuffer = nullptr;
    }

    if (m_instanceBuffer) {
        m_device.destroyBuffer(m_instanceBuffer);
        m_instanceBuffer = nullptr;
    }
}

void GL3InputAssembler::updateVertexData(const void* pData, uint32_t size,
                                         uint32_t vertexCount) {
    m_vertexBuffer->update(pData, size, 0);
    m_vertexCount = vertexCount;
}

void GL3InputAssembler::updateIndexData(const void* pData, uint32_t size,
                                        uint32_t indexCount) {
    m_indexBuffer->update(pData, size, 0);
    m_indexCount = indexCount;
}

void GL3InputAssembler::updateInstanceData(const void* pData, uint32_t size,
                                           uint32_t instanceCount) {
    m_instanceBuffer->update(pData, size, 0);
    m_instanceCount = instanceCount;
}

END_GFX_NAMESPACE