#pragma once

#include <time.h>

/**
 * SOURCE: https://www.youtube.com/@CasualCoder
 * in his snake series this was used for smoother movement.
 * I use it here as timer for how much time does human player have for his turn.
*/
class Time {
    public:
        /**
         * Using the gettimeofday function, the current time is 
         * obtained in microseconds. Then, this time is converted 
         * to milliseconds and returned.
         * tv_sec are seconds from epoch 1.1.1970
         * tv_usec are microseconds from last second
         * (timeNow.tv_sec * 1000) seconds -> miliseconds
         * (timeNow.tv_usec / 1000) microseconds -> miliseconds
        */
        time_t milliseconds();
};