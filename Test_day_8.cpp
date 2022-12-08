//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_8.h"

namespace Day8 {

    std::string testInput = R"(30373
25512
65332
33549
35390)";

    class TestDay8 : public ::testing::Test {
    public:
        TestDay8() : tm(testInput) {}

        TreeMap tm;
    };

    TEST_F(TestDay8, GetVisibleTrees) {
        ASSERT_EQ(tm.numVisibleTrees(), 21);
    }

    TEST(Day8Answer1, GetVisibleTrees) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_8.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        TreeMap tm(str);
        int num = tm.numVisibleTrees();

        std::cout << "[ DAY 8 ] Visible trees  = " << num << std::endl;
    }


}
