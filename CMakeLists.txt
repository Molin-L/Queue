cmake_minimum_required(VERSION 3.17)

project(QueueBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmark
)

file(GLOB_RECURSE TEST_SRC "tests/test_*.cpp")

enable_testing()
add_executable(test_queue ${TEST_SRC})
target_link_libraries(
    test_queue 
    GTest::gtest_main
    GTest::gtest
    pthread
)

# Generate DartConfiguration.tcl
configure_file(${CMAKE_ROOT}/Modules/DartConfiguration.tcl.in
               ${CMAKE_CURRENT_BINARY_DIR}/DartConfiguration.tcl @ONLY)

# Enable testing and add the test
add_test(NAME QueueTests COMMAND test_queue)
