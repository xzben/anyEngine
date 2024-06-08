#pragma once

#include "app/Application.h"
#include "platform/Window.h"

BEGIN_NS_SCENCE_GRAPH
Application* createRunApp(const char* appName, uint32_t width, uint32_t height);
END_NS_SCENCE_GRAPH