#ifndef CONTROL_TIMED_SHARED_PTR_H
#define CONTROL_TIMED_SHARED_PTR_H

#include <chrono>
#include <iostream>

using Clock = std::chrono::high_resolution_clock;
typedef std::chrono::time_point<Clock> clockTime;
typedef std::chrono::milliseconds milliSeconds;

struct ControlTimedSharedPtr {
    void* ptr;
    long myUseCount;
    clockTime TimedSharedPtrStartTime;
    milliSeconds deletePtrInMs;

    ControlTimedSharedPtr(void* p, long deleteMeInMilliseconds);
    ~ControlTimedSharedPtr();
};

#endif // CONTROL_TIMED_SHARED_PTR_H
