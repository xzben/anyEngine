#include "GL3Queue.h"

BEGIN_GFX_NAMESPACE
GL3Queue::GL3Queue(GL3Device& device) : m_device(device) {}
GL3Queue::~GL3Queue() {}

void GL3Queue::destroyCommandPool(CommandPool* pool) {}

END_GFX_NAMESPACE