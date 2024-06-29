#include "Mesh.h"

#include "app/Application.h"
BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Mesh)

Mesh::Mesh(gfx::PrimitiveType primitiveType) : m_primitiveType(primitiveType) {}

Mesh::~Mesh() {
    auto device = Application::getInstance()->getRenderSystem()->getDevice();
    if (m_inputAssembler != nullptr) {
        device->destroyObject(m_inputAssembler);
        m_inputAssembler = nullptr;
    }
}

gfx::InputAssembler* Mesh::getInput() {
    if (m_inputAssembler != nullptr && !m_isDirty) return m_inputAssembler;
    m_isDirty = false;

    auto device = Application::getInstance()->getRenderSystem()->getDevice();
    if (m_inputAssembler != nullptr) {
        device->destroyObject(m_inputAssembler);
        m_inputAssembler = nullptr;
    }

    if (m_indexCount > 0) {
        m_inputAssembler =
            device->createInputAssembler(m_primitiveType, m_attributes, m_vertexData.data(),
                                         m_vertexCount, m_indexData.data(), m_indexCount);
    } else {
        m_inputAssembler = device->createInputAssembler(m_primitiveType, m_attributes,
                                                        m_vertexData.data(), m_vertexCount);
    }

    return m_inputAssembler;
}

END_NS_SCENCE_GRAPH