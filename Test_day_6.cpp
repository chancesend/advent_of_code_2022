//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_6.h"

namespace Day6 {

    class TestDay6 : public ::testing::Test {
    public:
        TestDay6() {}
    };

    TEST_F(TestDay6, TestSopAndSomExamples) {
        std::vector<std::tuple<std::string, int, int>> data = {
                {"mjqjpqmgbljsphdztnvjfqwrcgsmlb", 7, 19},
                {"bvwbjplbgvbhsrlpgdmjqwftvncz", 5, 23},
                {"nppdvjthqldpwncqszvftbrmjlhg", 6, 23},
                {"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10, 29},
                {"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11, 26},
        };
        for(const auto [inStr, expectedSop, expectedSom]: data)
        {
            DataStream stream(inStr);
            ASSERT_EQ(stream.getSopMarkerPos(), expectedSop);
            ASSERT_EQ(stream.getSomMarkerPos(), expectedSom);
        }
    }

    TEST(Day6Answer1, StartOfPacketMarkerPos) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_6.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        DataStream ds(str);

        auto sop = ds.getSopMarkerPos();

        std::cout << "[ DAY 6 ] Start of packet marker pos  = " << sop << std::endl;
    }

    TEST(Day6Answer2, StartOfMessageMarkerPos) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_6.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        DataStream ds(str);

        auto sop = ds.getSomMarkerPos();

        std::cout << "[ DAY 6 ] Start of message marker pos  = " << sop << std::endl;
    }
}
