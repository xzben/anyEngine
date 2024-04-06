#pragma once

#include "../common/gfx_common.h"
#include "Shader.h"

BEGIN_GFX_NAMESPACE
class Device {
public:
    static Device* createDevice(const DeviceInfo& info);

public:
    Device()          = default;
    virtual ~Device() = default;

    virtual bool init(const DeviceInfo& info) = 0;

    virtual Shader* createShader() = 0;
};

END_GFX_NAMESPACE