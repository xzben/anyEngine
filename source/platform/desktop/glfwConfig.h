#pragma once
#include <unordered_map>

#include "event/Event.h"

extern const std::unordered_map<int, MouseButton> g_mapMouseButon;
extern const std::unordered_map<int, KeyCode> g_mapKeyCodeMap;
extern const std::unordered_map<int, PressStatus> g_pressMap;