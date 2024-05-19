#include "GL3InputAssembler.h"

#include "GL3Device.h"

BEGIN_GFX_NAMESPACE

static const uint32_t g_dataTypeCount[] = {
    1,   // DataFormat::Bool
    2,   // DataFormat::Bool2
    3,   // DataFormat::Bool3
    4,   // DataFormat::Bool4
    1,   // DataFormat::Int
    2,   // DataFormat::Int2
    3,   // DataFormat::Int3
    4,   // DataFormat::Int4
    1,   // DataFormat::UInt
    2,   // DataFormat::UInt2
    3,   // DataFormat::UInt3
    4,   // DataFormat::UInt4
    1,   // DataFormat::Float
    2,   // DataFormat::Float2
    3,   // DataFormat::Float3
    4,   // DataFormat::Float4
    16,  // DataFormat::Mat4
    9,   // DataFormat::Mat3
};

static const uint32_t g_dataFmtSize[] = {
    4,  // DataFormat::Bool
    4,  // DataFormat::Bool2
    4,  // DataFormat::Bool3
    4,  // DataFormat::Bool4
    4,  // DataFormat::Int
    4,  // DataFormat::Int2
    4,  // DataFormat::Int3
    4,  // DataFormat::Int4
    4,  // DataFormat::UInt
    4,  // DataFormat::UInt2
    4,  // DataFormat::UInt3
    4,  // DataFormat::UInt4
    4,  // DataFormat::Float
    4,  // DataFormat::Float2
    4,  // DataFormat::Float3
    4,  // DataFormat::Float4
    4,  // DataFormat::Mat4
    4,  // DataFormat::Mat3
};

static const GLenum g_attributeFmts[] = {
    GL_BOOL,          // DataFormat::Bool
    GL_BOOL,          // DataFormat::Bool2
    GL_BOOL,          // DataFormat::Bool3
    GL_BOOL,          // DataFormat::Bool4
    GL_INT,           // DataFormat::Int
    GL_INT,           // DataFormat::Int2
    GL_INT,           // DataFormat::Int3
    GL_INT,           // DataFormat::Int4
    GL_UNSIGNED_INT,  // DataFormat::UInt
    GL_UNSIGNED_INT,  // DataFormat::UInt2
    GL_UNSIGNED_INT,  // DataFormat::UInt3
    GL_UNSIGNED_INT,  // DataFormat::UInt4
    GL_FLOAT,         // DataFormat::Float
    GL_FLOAT,         // DataFormat::Float2
    GL_FLOAT,         // DataFormat::Float3
    GL_FLOAT,         // DataFormat::Float4
    GL_FLOAT,         // DataFormat::Mat4
    GL_FLOAT,         // DataFormat::Mat3
};

static const GLenum g_primitives[] = {
    GL_POINTS,                    //
    GL_LINES,                     //
    GL_LINE_STRIP,                //
    GL_TRIANGLES,                 //
    GL_TRIANGLE_STRIP,            //
    GL_TRIANGLE_FAN,              //
    GL_LINES_ADJACENCY,           //
    GL_LINE_STRIP_ADJACENCY,      //
    GL_TRIANGLES_ADJACENCY,       //
    GL_TRIANGLE_STRIP_ADJACENCY,  //
    GL_PATCHES,                   //
};

static const bool g_intType[] = {
    true,   // DataFormat::Bool
    true,   // DataFormat::Bool2
    true,   // DataFormat::Bool3
    true,   // DataFormat::Bool4
    true,   // DataFormat::Int
    true,   // DataFormat::Int2
    true,   // DataFormat::Int3
    true,   // DataFormat::Int4
    true,   // DataFormat::UInt
    true,   // DataFormat::UInt2
    true,   // DataFormat::UInt3
    true,   // DataFormat::UInt4
    false,  // DataFormat::Float
    false,  // DataFormat::Float2
    false,  // DataFormat::Float3
    false,  // DataFormat::Float4
    false,  // DataFormat::Mat4
    false,  // DataFormat::Mat3
};

void GL3InputAssembler::GetAttributeDataInfo(DataFormat format, uint32_t& count, uint32_t& size, GLenum& gltype, bool& intType) {
    int index = (int)format;
    count     = g_dataTypeCount[index];
    size      = g_dataFmtSize[index];
    gltype    = g_attributeFmts[index];
    intType   = g_intType[index];
}

GLenum GL3InputAssembler::GetPrimivteGLType(PrimitiveType type) { return g_primitives[(int)type]; }

GL3InputAssembler::GL3InputAssembler(GL3Device& device, PrimitiveType primitiveType, const std::vector<Attribute>& attributes,
                                     const void* pVertexData, uint32_t vertexCount, const void* pIndexData, uint32_t indexCount,
                                     uint32_t indexItemSize)
    : m_device(device),
      m_primitiveType(primitiveType),
      m_attributes(attributes),
      m_vertexCount(vertexCount),
      m_indexCount(indexCount),
      m_indexSize(indexItemSize) {
    m_vertexStripe = 0;
    for (auto& attr : m_attributes) {
        uint32_t itemSize = g_dataFmtSize[(int)attr.format];
        uint32_t count    = g_dataTypeCount[(int)attr.format];
        m_vertexStripe += itemSize * count;
    }
    m_vertexBuffer = new GL3Buffer(m_device, BufferType::VERTEX, vertexCount * m_vertexStripe, pVertexData);

    if (pIndexData != nullptr && indexCount > 0) {
        m_indexBuffer = new GL3Buffer(m_device, BufferType::INDEX, indexItemSize * indexCount, pIndexData);
    }
}

GL3InputAssembler::GL3InputAssembler(GL3Device& device, PrimitiveType primitiveType, const std::vector<Attribute>& attributes,
                                     const std::vector<Attribute>& InstanceAttributes, const void* pVertexData, uint32_t vertexCount,
                                     const void* pInstanceData, uint32_t instanceCount, const void* pIndexData, uint32_t indexCount,
                                     uint32_t indexItemSize)
    : GL3InputAssembler(device, primitiveType, attributes, pVertexData, vertexCount, pIndexData, indexCount, indexItemSize) {
    m_instanceAttributes = InstanceAttributes;
    m_instanceCount      = instanceCount;

    m_instanceStrip = 0;
    for (auto& attr : m_instanceAttributes) {
        uint32_t itemSize = g_dataFmtSize[(int)attr.format];
        uint32_t count    = g_dataTypeCount[(int)attr.format];
        m_instanceStrip += itemSize * count;
    }

    m_instanceBuffer = new GL3Buffer(m_device, BufferType::VERTEX, vertexCount * m_vertexStripe, pInstanceData);
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

void GL3InputAssembler::updateVertexData(const void* pData, uint32_t size, uint32_t vertexCount) {
    m_vertexBuffer->update(pData, size, 0);
    m_vertexCount = vertexCount;
}

void GL3InputAssembler::updateIndexData(const void* pData, uint32_t size, uint32_t indexCount) {
    m_indexBuffer->update(pData, size, 0);
    m_indexCount = indexCount;
}

void GL3InputAssembler::updateInstanceData(const void* pData, uint32_t size, uint32_t instanceCount) {
    m_instanceBuffer->update(pData, size, 0);
    m_instanceCount = instanceCount;
}

END_GFX_NAMESPACE