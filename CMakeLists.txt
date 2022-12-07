cmake_minimum_required(VERSION 3.23)
project(AdventOfCode2022 CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

add_library(current_day
        day_5.h)
set_target_properties(current_day PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
        current_tests
        Test_Day_5.cpp
)

add_library(previous_days
        day_4.h
        day_3.h
        day_2.h
        day_1.h)
set_target_properties(previous_days PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
        previous_tests
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
)
target_link_libraries(
        previous_tests
        PUBLIC
        GTest::gtest_main
        previous_days
)

target_compile_definitions(current_tests
        PUBLIC INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs/")
target_compile_definitions(previous_tests
        PUBLIC INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs/")

gtest_discover_tests(current_tests)
gtest_discover_tests(previous_tests)
