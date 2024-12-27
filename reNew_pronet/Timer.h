#pragma once
#include <iostream>
#include <chrono>

struct TimerInfo {
    std::chrono::microseconds timeMicro;
    std::chrono::milliseconds timeMill;
    time_t time;
};

class Timer
{
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> begTimeMicro;
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> begTimeMill;
    const time_t begTime;

public:

    Timer()
        : begTimeMicro(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()))
        , begTimeMill(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()))
        , begTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
    {
    }

    TimerInfo getTimeDiff() const {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> endTimeMicro(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()));
        std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> endTimeMill(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()));
        time_t endTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

        TimerInfo info{ 
            abs(std::chrono::duration_cast<std::chrono::microseconds>(endTimeMicro - begTimeMicro)),
            abs(std::chrono::duration_cast<std::chrono::milliseconds>(endTimeMill - begTimeMicro)),
            endTime - begTime
        };

        return info;
    }

    ~Timer() {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> endTimeMicro(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()));
        std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> endTimeMill(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()));
        time_t endTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

        std::chrono::microseconds durMicro;
        std::chrono::milliseconds durMilli;
        time_t durTime;

        durMicro = abs(std::chrono::duration_cast<std::chrono::microseconds>(endTimeMicro - begTimeMicro));
        durMilli = abs(std::chrono::duration_cast<std::chrono::milliseconds>(endTimeMill - begTimeMicro));
        durTime = endTime - begTime;

        std::cout << "duration time : " << durTime << "." << durMilli.count() << durMicro.count() << std::endl;
        std::cout << "end" << std::endl;
    }
};
