//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_9.h"

namespace Day9 {

    std::string testInput = R"(30373
25512
65332
33549
35390)";

    class TestDay9 : public ::testing::Test {
    public:
        TestDay9() : tv(testInput) {}

        TreeViewer tv;
    };

    TEST_F(TestDay9, GetVisibleTrees) {
        ASSERT_EQ(tv.numVisibleTrees(), 21);
    }

    TEST(Day9Answer1, xxx) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_9.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        TreeViewer tv(str);
        int num = tv.numVisibleTrees();

        std::cout << "[ DAY 9 ] xxx  = " << num << std::endl;
    }

    TEST(Day8Answer2, yyy) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_9.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        TreeViewer tv(str);
        int num = tv.getHighestScenicScore();

        std::cout << "[ DAY 8 ] yyy  = " << num << std::endl;
    }


}
