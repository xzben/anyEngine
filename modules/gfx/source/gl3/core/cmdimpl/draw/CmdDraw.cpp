#include "CmdDraw.h"

#include "../../GLContext.h"
#include "gl3/GL3InputAssembler.h"
#include "gl3/GL3Pipeline.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

static void bindInput(uint32_t vao, InputAssembler* input) {
    auto vbuffer     = input->getVertexBuffer();
    auto ibuffer     = input->getIndexBuffer();
    auto instanceBuf = input->getInstanceBuffer();

    auto& attributes = input->getAttributes();

    GL_CHECK(glBindVertexArray(vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbuffer->getHandle<OGL_HANDLE>()));

    uint32_t count;
    GLenum formatType;
    uint32_t formatSize = 0;
    bool intType        = false;
    for (auto& attr : attributes) {
        GL3InputAssembler::GetAttributeDataInfo(attr.format, count, formatSize, formatType,
                                                intType);
        if (count > 4) {
            int step        = std::ceil(count / 4);
            int location    = attr.location;
            int remainCount = count;
            int stepOffset  = 0;
            for (int i = 0; i < step; i++) {
                int curCount = remainCount > 4 ? 4 : remainCount;
                remainCount -= curCount;
                if (intType) {
                    GL_CHECK(
                        glVertexAttribIPointer(location + i, curCount, formatType, attr.stride,
                                               reinterpret_cast<void*>(attr.offset + stepOffset)));
                } else {
                    GL_CHECK(glVertexAttribPointer(
                        location + i, curCount, formatType, attr.isNormalized ? GL_TRUE : GL_FALSE,
                        attr.stride, reinterpret_cast<void*>(attr.offset + stepOffset)));
                }

                GL_CHECK(glVertexAttribDivisor(location + i, 0));
                stepOffset += curCount * formatSize;
            }
        } else {
            if (intType) {
                GL_CHECK(glVertexAttribIPointer(attr.location, count, formatType, attr.stride,
                                                reinterpret_cast<void*>(attr.offset)));
            } else {
                GL_CHECK(glVertexAttribPointer(attr.location, count, formatType,
                                               attr.isNormalized ? GL_TRUE : GL_FALSE, attr.stride,
                                               reinterpret_cast<void*>(attr.offset)));
            }
            GL_CHECK(glEnableVertexAttribArray(attr.location));
            GL_CHECK(glVertexAttribDivisor(attr.location, 0));
        }
    }

    if (instanceBuf) {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, instanceBuf->getHandle<OGL_HANDLE>()));
        auto& instanceAttribute = input->getInstanceAttributes();
        for (auto& attr : instanceAttribute) {
            GL3InputAssembler::GetAttributeDataInfo(attr.format, count, formatSize, formatType,
                                                    intType);
            if (count > 4) {
                int step        = std::ceil(count / 4);
                int location    = attr.location;
                int remainCount = count;
                int stepOffset  = 0;
                for (int i = 0; i < step; i++) {
                    int curCount = remainCount > 4 ? 4 : remainCount;
                    remainCount -= curCount;
                    if (intType) {
                        GL_CHECK(glVertexAttribIPointer(
                            location + i, curCount, formatType, attr.stride,
                            reinterpret_cast<void*>(attr.offset + stepOffset)));
                    } else {
                        GL_CHECK(glVertexAttribPointer(
                            location + i, curCount, formatType,
                            attr.isNormalized ? GL_TRUE : GL_FALSE, attr.stride,
                            reinterpret_cast<void*>(attr.offset + stepOffset)));
                    }

                    stepOffset += curCount * formatSize;
                    GL_CHECK(glEnableVertexAttribArray(location + i));
                    GL_CHECK(glVertexAttribDivisor(location + i, 1));
                }
            } else {
                if (intType) {
                    GL_CHECK(glVertexAttribIPointer(attr.location, count, formatType, attr.stride,
                                                    reinterpret_cast<void*>(attr.offset)));
                } else {
                    GL_CHECK(glVertexAttribPointer(
                        attr.location, count, formatType, attr.isNormalized ? GL_TRUE : GL_FALSE,
                        attr.stride, reinterpret_cast<void*>(attr.offset)));
                }

                GL_CHECK(glEnableVertexAttribArray(attr.location));
                GL_CHECK(glVertexAttribDivisor(attr.location, 1));
            }
        }
    }

    if (ibuffer) {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer->getHandle<OGL_HANDLE>()));
    }
}

void CmdDraw::execute(gl3::GLContext* context) {
    auto shader = m_pipeline->getShader();
    for (auto& item : m_uniformBuffers) {
        shader->setUniformBuffer(item.name, item.buffer, item.offset, item.size);
    }
    for (auto& item : m_storageBuffers) {
        shader->setShaderStorageBuffer(item.name, item.buffer, item.offset, item.size);
    }
    for (auto& item : m_textures) {
        shader->setTexture(item.name, item.texture, item.sampler);
    }
    context->updateState(m_pipeline->getState());
    shader->bind();

    m_vaos.resize(m_meshs.size());
    context->allocVao(m_vaos.size(), m_vaos.data());

    for (int i = 0; i < m_meshs.size(); i++) {
        auto& mesh           = m_meshs[i];
        auto input           = mesh.input;
        GLenum primitiveType = GL3InputAssembler::GetPrimivteGLType(input->getPrimitiveType());

        bindInput(m_vaos[i], input);
        auto vbuffer     = input->getVertexBuffer();
        auto ibuffer     = input->getIndexBuffer();
        auto instanceBuf = input->getInstanceBuffer();

        if (ibuffer == nullptr) {
            if (instanceBuf) {
                CCASSERT(mesh.instanceCount > 0, "instance count must > 0");
                GL_CHECK(glDrawArraysInstanced(primitiveType, mesh.offset, mesh.count,
                                               mesh.instanceCount));
            } else {
                GL_CHECK(glDrawArrays(primitiveType, mesh.offset, mesh.count));
            }
        } else {
            uint32_t indexItemSize = input->getIndexItemSize();
            GLenum indexType       = GL_UNSIGNED_INT;
            switch (indexItemSize) {
                case 8: {
                    indexType = GL_UNSIGNED_BYTE;
                    break;
                }
                case 16: {
                    indexType = GL_UNSIGNED_SHORT;
                    break;
                }
                case 32: {
                    indexType = GL_UNSIGNED_INT;
                    break;
                }
                default: {
                    CCERROR("unsupport index item size:%d", indexItemSize);
                }
            }

            if (instanceBuf) {
                CCASSERT(mesh.instanceCount > 0, "instance count must > 0");
                GL_CHECK(glDrawElementsInstanced(primitiveType, mesh.count, indexType,
                                                 reinterpret_cast<void*>(mesh.offset),
                                                 mesh.instanceCount));
            } else {
                GL_CHECK(glDrawElements(primitiveType, mesh.count, indexType,
                                        reinterpret_cast<void*>(mesh.offset)));
            }
        }

        GL_CHECK(glBindVertexArray(0));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE