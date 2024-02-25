#pragma once

#include <functional>

#include "gfx_defines.h"
#include "gfx_types.h"

BEGIN_GFX_NAMESPACE

class Device;

const uint32_t GFX_OBJECT_NULL = 0;
template <class GfxObjectHandle>
class GfxObject {
protected:
    using HandleDeleter = std::function<void(const Device &, GfxObjectHandle)>;

public:
    GfxObject(const GfxObject &rhs)            = delete;
    GfxObject &operator=(const GfxObject &rhs) = delete;

    GfxObject(const GfxObject &&rhs) noexcept
        : GfxObject(rhs.m_device, rhs.m_type, rhs.m_handle,
                    std::move(rhs.m_deleter)) {
        rhs.m_device  = nullptr;
        rhs.m_handle  = GFX_OBJECT_NULL;
        rhs.m_deleter = nullptr;
        rhs.m_type    = GfxObjectType::UnKnown;
    }

    GfxObject &operator=(const GfxObject &&rhs) noexcept {
        if (m_handle != GFX_OBJECT_NULL && m_deleter) {
            _deleter(*m_device, m_handle);
        }

        m_device  = rhs.m_device;
        m_handle  = rhs.m_handle;
        m_type    = rhs.m_type;
        m_deleter = std::move(rhs.m_deleter);

        rhs.m_device  = nullptr;
        rhs.m_handle  = GFX_OBJECT_NULL;
        rhs.m_deleter = nullptr;
        rhs.m_type    = GfxObjectType::UnKnown;

        return *this;
    }
    virtual ~GfxObject() {
        if (m_handle != GFX_OBJECT_NULL && m_deleter) {
            m_deleter(*m_device, m_handle);
        }
        m_handle = GFX_OBJECT_NULL;
    }

    operator GfxObjectHandle() const { return m_handle; }
    operator bool() const { return m_handle != GFX_OBJECT_NULL; }

protected:
    GfxObject(const Device &device, GfxObjectType type, GfxObjectHandle handle,
              HandleDeleter deleter) noexcept
        : m_device(&device),
          m_type(type),
          m_handle(handle),
          m_deleter(std::move(deleter)) {}

protected:
    const Device *m_device{nullptr};
    GfxObjectHandle m_handle{GFX_OBJECT_NULL};
    HandleDeleter m_deleter = nullptr;
    GfxObjectType m_type{GfxObjectType::UnKnown};
};

END_GFX_NAMESPACE