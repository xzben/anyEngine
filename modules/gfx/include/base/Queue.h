#pragma once

#include <vector>

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class CommandBuffer;
class CommandPool;
class Fence;
class SwapChain;
class Semaphore;

class Queue : public GfxObject {
public:
    virtual CommandPool* createPool(ResetMode resetModel = ResetMode::ResetPool) = 0;

    virtual bool submit(const std::vector<CommandBuffer*>& cmd, const std::vector<Semaphore*>& wait,
                        const std::vector<Semaphore*>& signal, Fence* fence = nullptr) = 0;
    virtual uint32_t present(SwapChain* swapChain, uint32_t imageIndex,
                             const std::vector<Semaphore*>& waits)                     = 0;
    virtual bool waitIdle()                                                            = 0;
    virtual QueueType getType()                                                        = 0;

protected:
    Queue() : GfxObject(GfxObjectType::Queue) {}
    virtual ~Queue() = default;
};

END_GFX_NAMESPACE