#pragma once

#include <vector>

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class CommandBuffer;
class Fence;
class SwapChain;
class Semaphore;

class Queue {
public:
    Queue()                                                         = default;
    virtual ~Queue()                                                = default;
    virtual bool submit(CommandBuffer* cmd, Fence* fence = nullptr) = 0;
    virtual uint32_t present(SwapChain* swapChain, uint32_t imageIndex,
                             const std::vector<Semaphore*>& waits)  = 0;
    virtual bool waitIdle()                                         = 0;
};

END_GFX_NAMESPACE