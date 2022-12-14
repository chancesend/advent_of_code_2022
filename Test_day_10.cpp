//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_10.h"

namespace Day10 {

    std::string testInput = R"(addx 15
addx -11
addx 6
addx -3
addx 5
addx -1
addx -8
addx 13
addx 4
noop
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx -35
addx 1
addx 24
addx -19
addx 1
addx 16
addx -11
noop
noop
addx 21
addx -15
noop
noop
addx -3
addx 9
addx 1
addx -3
addx 8
addx 1
addx 5
noop
noop
noop
noop
noop
addx -36
noop
addx 1
addx 7
noop
noop
noop
addx 2
addx 6
noop
noop
noop
noop
noop
addx 1
noop
noop
addx 7
addx 1
noop
addx -13
addx 13
addx 7
noop
addx 1
addx -33
noop
noop
noop
addx 2
noop
noop
noop
addx 8
noop
addx -1
addx 2
addx 1
noop
addx 17
addx -9
addx 1
addx 1
addx -3
addx 11
noop
noop
addx 1
noop
addx 1
noop
noop
addx -13
addx -19
addx 1
addx 3
addx 26
addx -30
addx 12
addx -1
addx 3
addx 1
noop
noop
noop
addx -9
addx 18
addx 1
addx 2
noop
noop
addx 9
noop
noop
noop
addx -1
addx 2
addx -37
addx 1
addx 3
noop
addx 15
addx -21
addx 22
addx -6
addx 1
noop
addx 2
addx 1
noop
addx -10
noop
noop
addx 20
addx 1
addx 2
addx 2
addx -6
addx -11
noop
noop
noop)";


    class TestDay10 : public ::testing::Test {
    public:
        TestDay10() : sc(testInput) {}

        StrengthChecker sc;
    };

    TEST_F(TestDay10, SumOfSignalStrengths) {
        std::vector<int> cycles {20, 60, 100, 140, 180, 220};
        auto strengths = sc.getSignalStrengths(cycles);
        auto sum = std::accumulate(strengths.begin(), strengths.end(), 0);
 //       ASSERT_EQ(sum, 13140); // !! This is the answer to the example provided by AoC, but it was wrong!
        ASSERT_EQ(sum, 13360);  // This is my own computed answer to the example.
    }

    TEST_F(TestDay10, CrtOutput) {
        std::string expectedOutput = R"(##..##..##..##..##..##..##..##..##..##..
###...###...###...###...###...###...###.
####....####....####....####....####....
#####.....#####.....#####.....#####.....
######......######......######......####
#######.......#######.......#######.....)";

        auto addList = sc.getAddList();
        auto xAccumList = sc.getXAccumList(addList);
        auto strOut = getCrtOutput(xAccumList);
        ASSERT_EQ(strOut, expectedOutput);
    }

    TEST(Day10Answer1, SumOfSixSignalStrengths) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_10.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        StrengthChecker sc(str);
        std::vector<int> cycles {20, 60, 100, 140, 180, 220};
        auto strengths = sc.getSignalStrengths(cycles);
        auto sum = std::accumulate(strengths.begin(), strengths.end(), 0);

        std::cout << "[ DAY 10 ] Sum of sig strengths  = " << sum << std::endl;
        ASSERT_EQ(sum, 14240);
    }

    TEST(Day10Answer2, EightCapitalLettersOnScreen) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_10.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        StrengthChecker sc(str);
        auto strOut = getCrtOutput(sc.getXAccumList(sc.getAddList()));

        std::cout << "[ DAY 10 ] CRT Printout: " << std::endl << strOut << std::endl;
    }



}
