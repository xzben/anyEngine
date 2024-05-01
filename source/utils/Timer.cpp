#include "Timer.h"

#include <chrono>
#include <ctime>

time_t Timer::get_time_stamp_sec() {
    return get_time_stamp_microsec() / 1000000;
}

time_t Timer::get_time_stamp_millisec() {
    return get_time_stamp_microsec() / 1000;
}

time_t Timer::get_time_stamp_microsec() {
    std::chrono::time_point time_point_now =
        std::chrono::system_clock::now();  // ��ȡ��ǰʱ���
    std::chrono::system_clock::duration duration_since_epoch =
        time_point_now
            .time_since_epoch();  // ��1970-01-01 00:00:00����ǰʱ����ʱ��
    time_t microseconds_since_epoch =
        std::chrono::duration_cast<std::chrono::microseconds>(
            duration_since_epoch)
            .count();  // ��ʱ��ת��Ϊ΢����

    return microseconds_since_epoch;
}

Timer::Timer() {
    std::chrono::time_point time_point_now =
        std::chrono::system_clock::now();  // ��ȡ��ǰʱ���
    std::chrono::system_clock::duration duration_since_epoch =
        time_point_now
            .time_since_epoch();  // ��1970-01-01 00:00:00����ǰʱ����ʱ��
    time_t microseconds_since_epoch =
        std::chrono::duration_cast<std::chrono::microseconds>(
            duration_since_epoch)
            .count();  // ��ʱ��ת��Ϊ΢����
    time_t seconds_since_epoch =
        microseconds_since_epoch / 1000000;  // ��ʱ��ת��Ϊ����

    tm_microsec = microseconds_since_epoch % 1000;
    tm_millisec = microseconds_since_epoch / 1000 % 1000;
#if defined _MSC_VER && _MSC_VER >= 1400
    localtime_s(this, &seconds_since_epoch);
#elif defined __GNUC__
    localtime_r(&seconds_since_epoch, this);
#else
    std::tm *self = this;
    *self         = *std::localtime(&seconds_since_epoch);
#endif
}