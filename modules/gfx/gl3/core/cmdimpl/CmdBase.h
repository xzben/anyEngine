#pragma once

#include "../gl3_core_common.h"
#include "NoCopy.h"

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
    DISPATCH,
    NEXT_SUBPASS,
    END_RENDERPASS,
    SET_VIEWPORT,
    SET_SCISSOR,
    UPDATE_BUFFER,
    UPDATE_TEXUTRE,
    COPY_BUFFER,
    COPY_TEXTURE,
    BLIT_TEXTURE,
    GENERATE_MIPMAP,
};

class CmdBase : public NoCopy {
public:
    CmdBase(GL3CommandBuffer& cmdBuf, CmdType type);
    virtual ~CmdBase();

    virtual void reset()   = 0;
    virtual void execute() = 0;

    CmdType getType() { return m_type; }

protected:
    GL3CommandBuffer& m_cmdBuf;
    CmdType m_type;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE