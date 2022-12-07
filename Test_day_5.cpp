//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_5.h"

namespace Day5 {

    std::string test_data = R"(    [D]
[N] [C]
[Z] [M] [P]
 1   2   3

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2
)";

    class TestDay5 : public ::testing::Test {
    public:
        TestDay5() : cm(test_data, Move::Type::CM_9000) {}

        CrateMover cm;
    };

    TEST_F(TestDay5, ReadStacks) {
        std::string strStack = R"(    [D]
[N] [C]
[Z] [M] [P]
 1   2   3
)";
        CrateStacks stacks(strStack);
        ASSERT_EQ(stackToString(stacks.getStack(0)), "ZN");
        ASSERT_EQ(stackToString(stacks.getStack(1)), "MCD");
        ASSERT_EQ(stackToString(stacks.getStack(2)), "P");
        ASSERT_EQ(stacks.getTopOfStacks(), "NDP");
    }

    TEST_F(TestDay5, GetTopOfStacks) {
        ASSERT_EQ(cm.getTopOfStacks(), "CMZ");
    }

    TEST_F(TestDay5, GetTopOfStacks_CM9001) {
        CrateMover cm9001(test_data, Move::Type::CM_9001);
        ASSERT_EQ(cm9001.getTopOfStacks(), "MCD");
    }

    TEST(Day5Answer1, TopOfStacksPostMove_CM9000) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_5.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        CrateMover cm(str, Move::Type::CM_9000);

        auto top = cm.getTopOfStacks();

        std::cout << "[ DAY 5 ] Top of stacks CM9000  = " << top << std::endl;
    }

    TEST(Day5Answer2, TopOfStacksPostMove_CM9001) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_5.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        CrateMover cm(str, Move::Type::CM_9001);

        auto top = cm.getTopOfStacks();

        std::cout << "[ DAY 5 ] Top of stacks CM9001  = " << top << std::endl;
    }
}
