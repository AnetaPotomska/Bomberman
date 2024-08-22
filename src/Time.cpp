#include "Time.h"

#include <chrono>
#include <sys/time.h>
#include <time.h>

time_t Time::milliseconds() {
    struct timeval timeNow {};
    gettimeofday(&timeNow, nullptr);
    return (timeNow.tv_sec * 1000) + (timeNow.tv_usec / 1000);
}