#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Shader : public GfxObject {
public:
    virtual bool addStage(const std::vector<uint8_t>& code,
                          gfx::ShaderStage stage,
                          const std::string& entryName = "main") = 0;
    virtual bool build()                                         = 0;

protected:
    Shader() : GfxObject(GfxObjectType::Shader) {}
    virtual ~Shader() = default;
};

END_GFX_NAMESPACE