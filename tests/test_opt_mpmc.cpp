 /* 
 * --------------------------------------------------------------
 * File:          test_opt_mpmc.cpp
 * Project:       Code
 * Created:       Friday, 17th January 2025 1:54:37 pm
 * @Author:       Molin Liu
 * Contact:       molinliu@live.com
 * Modified:      Friday, 17th January 2025 1:54:39 pm
 * Copyright      © Afterlife 2023 - 2025
 * --------------------------------------------------------------
 */ 

#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <atomic>
#include <vector>
#include "mpmc/opt_mpmc.h"
#include "time_utils.h"

struct Data {
    int64_t t0;
    int64_t data;

    bool operator==(const Data& other) const {
        return t0 == other.t0 && data == other.data;
    }

    bool operator!=(const Data& other) const {
        return !(*this == other);
    }

    Data& operator=(const Data& other) {
        t0 = other.t0;
        data = other.data;
        return *this;
    }
};

TEST(OptMPMC, Latency) {
    size_t n = (size_t)(1e4);
    OptimizedMPMCQueue<Data> queue(1024);
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    int thread_num = 5;
    std::vector<int64_t> durations(thread_num, 0);
    std::atomic<int64_t> count(0);

    bool running = true;
    
    for (size_t i = 0; i < thread_num; i++) {
        consumers.push_back(std::thread([&]() {
            while (running) {
                auto data = queue.pop();
                if (data.has_value()) {
                    int64_t diff = get_current_monotonic_nanoseconds() - data->t0;
                    durations[i] += diff;
                    if (diff > 10000) {
                        std::cerr << "diff: " << diff << std::endl;
                    }
                    count.fetch_add(1);
                }
            }
        }));
    }

    for (size_t i = 0; i < thread_num; i++) {
        producers.push_back(std::thread([&]() {
            for (size_t j = i; j < n; j+=thread_num) {
                queue.push(Data{get_current_monotonic_nanoseconds(), static_cast<int64_t>(j)});
            }
        }));
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    running = false;

    for (auto& thread : producers) {
        thread.join();
    }
    for (auto& thread : consumers) {
        thread.join();
    }

    int64_t duration_sum = 0;
    for (size_t i = 0; i < thread_num; i++) {
        duration_sum += durations[i];
    }

    std::cout << "Average duration: " << duration_sum / count << " ns" << std::endl;
}