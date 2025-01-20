 /* 
 * --------------------------------------------------------------
 * File:          dto.h
 * Project:       Code
 * Created:       Tuesday, 3rd September 2024 2:19:26 pm
 * @Author:       Molin Liu
 * Contact:       molinliu@live.com
 * Modified:      Tuesday, 3rd September 2024 2:21:03 pm
 * Copyright      © Afterlife 2023 - 2024
 * --------------------------------------------------------------
 */ 
#pragma once
#include "utils.h"

struct Data {
    int64_t create_ts_ns = 0;
    int64_t consume_ts_ns = 0;
    int64_t data;

    Data(int64_t data) : data(data) {
        create_ts_ns = get_current_nanoseconds();
    }

    Data() {
        create_ts_ns = get_current_nanoseconds();
        consume_ts_ns = 0;
        data = 0;
    }

    Data& operator=(const Data& other) {
        create_ts_ns = other.create_ts_ns;
        consume_ts_ns = other.consume_ts_ns;
        data = other.data;
        return *this;
    }
};
