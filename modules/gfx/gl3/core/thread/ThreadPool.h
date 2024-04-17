#pragma once

#include <thread>

#include "gl3/core/gl3_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class ThreadPool {
protected:
    void threadFunc();

public:
    ThreadPool(uint32_t threadNum);
    ~ThreadPool();

    void exit();
    void addTask();

private:
    std::vector<std::thread*> m_threads;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE