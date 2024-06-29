#pragma once

#include "ByteArray.h"
#include "common/Object.h"
#include "gfx.h"

BEGIN_NS_SCENCE_GRAPH
class Mesh : public Object {
    DECLARE_RUNTIME_CLASS(Mesh)
public:
    Mesh(gfx::PrimitiveType primitiveType);
    virtual ~Mesh();
    virtual void setAttributes(const std::vector<gfx::Attribute>& attributes) {
        m_attributes = attributes;
    }
    virtual void setPrimitiveType(gfx::PrimitiveType primitiveType) {
        m_primitiveType = primitiveType;
    }

    void setVertexCount(uint32_t vertexCount) { m_vertexCount = vertexCount; }
    void setIndexCount(uint32_t indexCount) { m_indexCount = indexCount; }
    uint32_t getVertexCount() const { return m_vertexCount; }
    uint32_t getIndexCount() const { return m_indexCount; }

    ByteArray& getVertexData() { return m_vertexData; }
    ByteArray& getIndexData() { return m_indexData; }
    void setDirty(bool dirty) { m_isDirty = dirty; }
    virtual gfx::InputAssembler* getInput();

protected:
    std::vector<gfx::Attribute> m_attributes;
    gfx::PrimitiveType m_primitiveType = gfx::PrimitiveType::TRIANGLE_LIST;
    ByteArray m_vertexData;
    ByteArray m_indexData;
    bool m_isDirty                        = true;
    uint32_t m_vertexCount                = 0;
    uint32_t m_indexCount                 = 0;
    gfx::InputAssembler* m_inputAssembler = nullptr;
};

END_NS_SCENCE_GRAPH