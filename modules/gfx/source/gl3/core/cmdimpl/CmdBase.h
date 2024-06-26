#pragma once

#include "../GLContext.h"
#include "../gl3_core_common.h"
#include "NoCopy.h"
#include "base/CommandBuffer.h"
#include "base/DrawSurface.h"

BEGIN_GFX_NAMESPACE

class GL3CommandBuffer;

BEGIN_GL3_CORE_NAMESPACE

enum class CmdType {
    BEGIN_RENDERPASS,
    BIND_PIPELINE,
    BIND_INPUTASSEMBER,
    BIND_TEXTURE,
    BIND_BUFFER,
    DRAW,
    COMPUTE,
    NEXT_SUBPASS,
    END_RENDERPASS,
    SET_VIEWPORT,
    SET_SCISSOR,
    UPDATE_BUFFER,
    UPDATE_INPUTASSEMBLER,
    UPDATE_TEXUTRE,
    COPY_BUFFER,
    COPY_TEXTURE,
    BLIT_TEXTURE,
    CLEAR_TEXTURE,
    GENERATE_MIPMAP,

    ENABLE,
    DISABLE,
};

class CmdBase : public NoCopy {
public:
    CmdBase(GL3CommandBuffer& cmdBuf, CmdType type);
    virtual ~CmdBase() = default;

    virtual void reset()                          = 0;
    virtual void execute(gl3::GLContext* context) = 0;

    CmdType getType() { return m_type; }

protected:
    GL3CommandBuffer& m_cmdBuf;
    CmdType m_type;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE