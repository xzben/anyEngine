#pragma once

#include "../base/Semaphore.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Semaphore : public Semaphore {
public:
    GL3Semaphore(GL3Device& device);
    virtual ~GL3Semaphore();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE