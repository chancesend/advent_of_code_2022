//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "day_14.h"

namespace Day14 {

    std::string testInput = R"(498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9)";


    class TestDay14 : public ::testing::Test {
    public:
        TestDay14() : ss(testInput) {}

        SandSimulator ss;
    };

    TEST_F(TestDay14, UnitsOfSandAtRestBeforeStartsFlowingToAbyss) {
        Pos start(500,0);
        auto units = ss.getNumSandBeforeFlowingIntoAbyss(start);
        ASSERT_EQ(units, 24);
    }

    TEST_F(TestDay14, UnitsOfSandAtRestBeforeStoppedUp) {
        Pos start(500,0);
        auto units = ss.getNumSandUntilSourceBlocked(start);
        ASSERT_EQ(units, 24);
    }

    TEST(Day14Answer1, SandUnitsBeforeAbyss) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_14.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        SandSimulator ss(str);
        Pos start(500,0);
        auto units = ss.getNumSandBeforeFlowingIntoAbyss(start);

        std::cout << "[ DAY 14 ] Units of sand before flow to abyss  = " << units << std::endl;
        ASSERT_EQ(units, 1298);
    }

    TEST(Day14Answer2, SandUnitsBeforeStoppedUp) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_14.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        SandSimulator ss(str);
        Pos start(500,0);
        auto units = ss.getNumSandUntilSourceBlocked(start);

        std::cout << "[ DAY 14 ] Units of sand before source is stopped  = " << units << std::endl;
        ASSERT_EQ(units, 25585);
    }


}
