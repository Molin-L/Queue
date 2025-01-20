 /* 
 * --------------------------------------------------------------
 * File:          time_utils.h
 * Project:       Code
 * Created:       Friday, 17th January 2025 11:10:44 am
 * @Author:       Molin Liu
 * Contact:       molinliu@live.com
 * Modified:      Friday, 17th January 2025 11:10:46 am
 * Copyright      © Afterlife 2023 - 2025
 * --------------------------------------------------------------
 */ 
#pragma once
#include <chrono>

inline int64_t get_current_nanoseconds() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

inline int64_t get_current_monotonic_nanoseconds() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}