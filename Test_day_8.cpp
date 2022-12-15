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
        TestDay8() : tv(testInput) {}

        TreeViewer tv;
    };

    TEST_F(TestDay8, GetVisibleTrees) {
        auto visible = tv.numVisibleTrees();
        ASSERT_EQ(visible, 21);
    }

    TEST_F(TestDay8, GetScenicScore) {
        ASSERT_EQ(tv.getScenicScore({1, 2}), 4);
        ASSERT_EQ(tv.getScenicScore({3, 2}), 8);
    }

    TEST_F(TestDay8, GetHighestScenicScore) {
        ASSERT_EQ(tv.getHighestScenicScore(), 8);
    }

    TEST(Day8Answer1, GetVisibleTrees) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_8.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        TreeViewer tv(str);
        int num = tv.numVisibleTrees();

        std::cout << "[ DAY 8 ] Visible trees  = " << num << std::endl;
        ASSERT_EQ(num, 1859);
    }

    TEST(Day8Answer2, GetHighestScenicScore) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_8.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        TreeViewer tv(str);
        int num = tv.getHighestScenicScore();

        std::cout << "[ DAY 8 ] Highest scenic score  = " << num << std::endl;
        ASSERT_EQ(num, 332640);
    }


}
