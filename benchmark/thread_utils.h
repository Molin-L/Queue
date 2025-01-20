 /* 
 * --------------------------------------------------------------
 * File:          thread_utils.h
 * Project:       Code
 * Created:       Tuesday, 3rd September 2024 3:10:03 pm
 * @Author:       Molin Liu
 * Contact:       molinliu@live.com
 * Modified:      Tuesday, 3rd September 2024 3:10:32 pm
 * Copyright      © Afterlife 2023 - 2024
 * --------------------------------------------------------------
 */ 

#pragma once

#include <atomic>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>

#include <sys/syscall.h>

namespace Common
{

inline std::string getThreadName() {
    char name[16];
    pthread_getname_np(pthread_self(), name, sizeof(name));
    return std::string(name);
}

inline void setThreadName(const std::string &name) {
    std::cerr << "[WARNING] " << "Set thread name: " << name << " " << pthread_self() << std::endl;
    pthread_setname_np(name.c_str());
}

/**
 * @brief Set affinity for the current thread to be pinned to the provided core_id.
 *
 * This function sets the CPU affinity for the current thread, pinning it to the specified core.
 * On macOS, CPU affinity cannot be set, so this function returns true without any action.
 *
 * @param core_id The ID of the core to which the thread should be pinned.
 * @return True if the affinity was set successfully (or not needed on macOS), false otherwise.
 */
inline auto setThreadCore(int core_id) noexcept
{
#ifdef __APPLE__
    // MacOS Cannot set affinity
    return true;
#else
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    std::string name = getThreadName();
    std::cerr << ORANGE("[WARNING] ") << "Set core affinity for " << GREEN(name) << " " << pthread_self() << " to " << GREEN(core_id) << std::endl;

    return (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) == 0);
#endif
}

/**
 * @brief Create and start a new thread with specified core affinity and function.
 *
 * This function creates and starts a new thread with the specified core affinity, name, function, and arguments.
 * It also waits for 1 second to allow the new thread to set its core affinity.
 *
 * @tparam T The type of the function to execute.
 * @tparam A The types of the function's arguments.
 * @param core_id The ID of the core to which the thread should be pinned.
 * @param name The name of the thread.
 * @param func The function to execute in the new thread.
 * @param args The arguments to pass to the function.
 * @return A pointer to the newly created thread.
 */
template<typename T, typename... A>
inline auto createAndStartThread(int core_id, const std::string &name, T &&func, A &&...args) noexcept
{
    auto t = new std::thread([&]() {
        setThreadName(name);
        if (core_id >= 0 && !setThreadCore(core_id))
        {
            std::cerr << "Failed to set core affinity for " << name << " " << pthread_self() << " to " << core_id
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        std::forward<T>(func)((std::forward<A>(args))...);
    });

    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return t;
}
} // namespace Common
