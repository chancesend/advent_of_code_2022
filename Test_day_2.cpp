//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_2.h"

namespace Day2 {

    std::string test_data = R"(A Y
B X
C Z
)";

    class TestDay2 : public ::testing::Test {
    public:
        TestDay2() : sg(test_data, Round::OldParse) {}

        StrategyGuide sg;
    };

    TEST_F(TestDay2, TotalScore) {
        ASSERT_EQ(sg.getTotalScore(), 15);
    }

    class TestDay2NewParse : public ::testing::Test {
    public:
        TestDay2NewParse() : sg(test_data, Round::NewParse) {}

        StrategyGuide sg;
    };

    TEST_F(TestDay2NewParse, TotalScore) {
        ASSERT_EQ(sg.getTotalScore(), 12);
    }

    TEST(Day2Answer1, TotalScoreForStrategyGuideOldParse) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_2.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        StrategyGuide sg(str, Round::OldParse);

        auto score = sg.getTotalScore();
        std::cout << "[ DAY 2 ] Total Score  = " << score << std::endl;
        ASSERT_EQ(score, 10994);
    }

    TEST(Day2Answer2, TotalScoreForStrategyGuideNewParse) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_2.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        StrategyGuide sg(str, Round::NewParse);

        auto score = sg.getTotalScore();
        std::cout << "[ DAY 2 ] Total Score  = " << score << std::endl;
        ASSERT_EQ(score, 12526);
    }

}