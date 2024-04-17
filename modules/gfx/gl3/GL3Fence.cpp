#include "GL3Fence.h"

BEGIN_GFX_NAMESPACE

GL3Fence::GL3Fence(GL3Device& device) : m_device(device) {}
GL3Fence::~GL3Fence() {}

bool GL3Fence::reset() { return true; }
bool GL3Fence::wait(uint32_t timeout) { return true; }
void GL3Fence::signal() {}

END_GFX_NAMESPACE