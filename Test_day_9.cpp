//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_9.h"

namespace Day9 {

    std::string testInput = R"(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2)";

    std::string testInput2 = R"(R 5
U 8
L 8
D 3
R 17
D 10
L 25
U 20)";

    class TestDay9 : public ::testing::Test {
    public:
        TestDay9() : rm(testInput, 2) {}

        RopeMover rm;
    };

    TEST_F(TestDay9, CountTailVisits_2Knots) {
        ASSERT_EQ(rm.numPlacesTailVisited(), 13);
    }

    TEST_F(TestDay9, CountTailVisits_10Knots)
    {
        {
            RopeMover rm10(testInput, 10, Board::Dims(10, 10));
            ASSERT_EQ(rm10.numPlacesTailVisited(), 1);
        }
        {
            RopeMover rm10(testInput2, 10, Board::Dims(100, 100));
            ASSERT_EQ(rm10.numPlacesTailVisited(), 36);
        }
    }

    TEST(Day9Answer1, CountTailVisits) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_9.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        RopeMover rm(str, 2, Board::Dims(1000,1000));
        int num = rm.numPlacesTailVisited();

        std::cout << "[ DAY 9 ] Num places 2-knot tail visited  = " << num << std::endl;
        ASSERT_EQ(num, 6037);
    }

    TEST(Day8Answer2, CountTailVisits_9Knots) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_9.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        RopeMover rm(str, 10, Board::Dims(1000,1000));
        int num = rm.numPlacesTailVisited();

        std::cout << "[ DAY 8 ] Num places 10-knot tail visited  = " << num << std::endl;
        ASSERT_EQ(num, 2485);
    }


}
