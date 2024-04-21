#pragma once

#include "../base/InputAssembler.h"
#include "GL3Buffer.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3InputAssembler : public InputAssembler {
public:
    GL3InputAssembler(GL3Device& device,
                      const std::vector<Attribute>& attributes,
                      const void* pVertexData, uint32_t vertexCount,
                      const void* pIndexData = nullptr, uint32_t indexCount = 0,
                      uint32_t indexItemSize = sizeof(uint32_t));
    GL3InputAssembler(GL3Device& device,
                      const std::vector<Attribute>& attributes,
                      const std::vector<Attribute>& InstanceAttributes,
                      const void* pVertexData, uint32_t vertexCount,
                      const void* pInstanceData, uint32_t instanceCount,
                      const void* pIndexData = nullptr, uint32_t indexCount = 0,
                      uint32_t indexItemSize = sizeof(uint32_t));
    virtual ~GL3InputAssembler();

    virtual const std::vector<Attribute>& getAttributes() const override {
        return m_attributes;
    }
    virtual GL3Buffer* getVertexBuffer() const override {
        return m_vertexBuffer;
    }
    virtual GL3Buffer* getIndexBuffer() const override { return m_indexBuffer; }
    virtual GL3Buffer* getInstanceBuffer() const override {
        return m_instanceBuffer;
    }
    virtual uint32_t getVertexCount() const override { return m_vertexCount; }
    virtual uint32_t getIndexCount() const override { return m_indexCount; }
    virtual uint32_t getVertexItemSize() const override {
        return m_vertexStripe;
    }
    virtual uint32_t getIndexItemSize() const override { return m_indexSize; }

    virtual void updateVertexData(const void* pData, uint32_t size,
                                  uint32_t vertexCount) override;
    virtual void updateIndexData(const void* pData, uint32_t size,
                                 uint32_t indexCount) override;
    virtual void updateInstanceData(const void* pData, uint32_t size,
                                    uint32_t instanceCount) override;

private:
    GL3Device& m_device;

    std::vector<Attribute> m_attributes;
    std::vector<Attribute> m_instanceAttributes;

    GL3Buffer* m_vertexBuffer{nullptr};
    GL3Buffer* m_indexBuffer{nullptr};
    GL3Buffer* m_instanceBuffer{nullptr};

    uint32_t m_vertexCount{0};
    uint32_t m_indexCount{0};
    uint32_t m_instanceCount{0};

    uint32_t m_vertexStripe{0};
    uint32_t m_instanceStrip{0};
    uint32_t m_indexSize{sizeof(int32_t)};
};

END_GFX_NAMESPACE