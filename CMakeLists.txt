cmake_minimum_required(VERSION 3.23)
project(AdventOfCode2022 CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(cmake)
enable_testing()

add_library(current_day
        day_12.h
        )
set_target_properties(current_day PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
        current_tests
        Test_Day_12.cpp
)

add_library(previous_days
        day_11.h
        day_10.h
        day_9.h
        day_8.h
        day_7.h
        day_6.h
        day_5.h
        day_4.h
        day_3.h
        day_2.h
        day_1.h)
set_target_properties(previous_days PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
        previous_tests
        Test_Day_11.cpp
        Test_Day_10.cpp
        Test_Day_9.cpp
        Test_Day_8.cpp
        Test_Day_7.cpp
        Test_Day_6.cpp
        Test_day_5.cpp
        Test_day_4.cpp
        Test_Day_3.cpp
        Test_Day_2.cpp
        Test_Day_1.cpp
)

target_link_libraries(
        current_tests
        PUBLIC
        GTest::gtest_main
        current_day
        Eigen3::Eigen
)
target_link_libraries(
        previous_tests
        PUBLIC
        GTest::gtest_main
        previous_days
        Eigen3::Eigen
)

target_compile_definitions(current_tests
        PUBLIC INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs/")
target_compile_definitions(previous_tests
        PUBLIC INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs/")

gtest_discover_tests(current_tests)
gtest_discover_tests(previous_tests)
