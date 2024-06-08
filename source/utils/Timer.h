#pragma once
#include <ctime>

#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
class Timer : public std::tm {
public:
    static time_t get_time_stamp_sec();
    static time_t get_time_stamp_millisec();
    static time_t get_time_stamp_microsec();

    Timer();
    int get_year() { this->tm_year; }
    int tm_millisec;
    int tm_microsec;
};

END_NS_SCENCE_GRAPH