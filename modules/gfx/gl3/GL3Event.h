#pragma once

#include "../base/Event.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Event : public Event {
public:
    GL3Event(GL3Device& device);
    virtual ~GL3Event();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE