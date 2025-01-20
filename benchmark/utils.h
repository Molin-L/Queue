 /* 
 * --------------------------------------------------------------
 * File:          utils.h
 * Project:       Code
 * Created:       Tuesday, 3rd September 2024 3:13:48 pm
 * @Author:       Molin Liu
 * Contact:       molinliu@live.com
 * Modified:      Tuesday, 3rd September 2024 3:13:50 pm
 * Copyright      © Afterlife 2023 - 2024
 * --------------------------------------------------------------
 */ 
#pragma once
#include <chrono>

inline int64_t get_current_nanoseconds()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

/**
 * @def ASSERT(condition, message)
 * @brief A macro for asserting a condition and exiting the program if it fails.
 *
 * @param condition The condition to be checked.
 * @param message The error message to display if the condition fails.
 */
#define ASSERT(condition, message)                                                                                     \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        std::cerr << "Assertion failed: " << message << std::endl;                                                     \
        exit(EXIT_FAILURE);                                                                                            \
    }

#ifndef DISALLOW_COPY_AND_ASSIGN
/**
 * @def DISALLOW_COPY_AND_ASSIGN(TypeName)
 * @brief A macro to disallow copy and move construction and assignment for a class.
 *
 * @param TypeName The name of the class to disallow copy and move operations.
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName)                                                                             \
    TypeName(const TypeName &) = delete;                                                                               \
    TypeName(const TypeName &&) = delete;                                                                              \
    TypeName &operator=(const TypeName &) = delete;                                                                    \
    TypeName &operator=(const TypeName &&) = delete;
#endif
