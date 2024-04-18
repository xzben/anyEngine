#include "CmdBase.h"

#include "gl3/GL3CommandBuffer.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

CmdBase::CmdBase(GL3CommandBuffer& cmdBuf, CmdType type)
    : m_cmdBuf(cmdBuf), m_type(type) {}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE