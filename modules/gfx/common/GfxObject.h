#pragma once

#include <atomic>
#include <functional>

#include "gfx_types.h"

BEGIN_GFX_NAMESPACE

class GPUObject {};
class GfxObject {
public:
    GfxObject(GfxObjectType type) : m_type(type) {}

    void addRef() { m_refCount++; }
    void delRef() {
        assert(m_refCount > 0);

        m_refCount--;
        if (m_refCount <= 0) {
            delete this;
        }
    }

    GfxObject(const GfxObject &rhs)            = delete;
    GfxObject &operator=(const GfxObject &rhs) = delete;

    GfxObject(const GfxObject &&rhs)            = delete;
    GfxObject &operator=(const GfxObject &&rhs) = delete;

protected:
    virtual ~GfxObject() = default;
    GfxObjectType m_type{GfxObjectType::UnKnown};
    std::atomic<uint32_t> m_refCount{1};
};

END_GFX_NAMESPACE