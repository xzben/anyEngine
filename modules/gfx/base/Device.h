#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Device {
public:
    Device()          = default;
    virtual ~Device() = default;

    virtual bool init(const DeviceInfo& info) = 0;
};

END_GFX_NAMESPACE