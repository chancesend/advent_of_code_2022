//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_1.h"

namespace Day1 {

    std::string test_data = R"(1000
2000
3000

4000

5000
6000

7000
8000
9000

10000
)";

    class TestDay1 : public ::testing::Test {
    public:
        TestDay1() : cc(test_data) {}

        CalorieCounter cc;
    };

    TEST_F(TestDay1, num_elves) {
        ASSERT_EQ(cc.getNumElves(), 5);
    }

    TEST_F(TestDay1, num_calories_for_elf) {
        ASSERT_EQ(cc.getNumCaloriesForElf(0), 6000);
    }

    TEST_F(TestDay1, max_elf_calories) {
        auto maxElf = cc.getElfWithMaxCalories();
        auto cals = maxElf.getNumCalories();
        ASSERT_EQ(cals, 24000);
    }

    TEST_F(TestDay1, ordered_elves) {
        auto orderedElves = cc.orderElvesByCalories();
        ASSERT_EQ(orderedElves[0].getNumCalories(), 24000);
        ASSERT_EQ(orderedElves[1].getNumCalories(), 11000);
        ASSERT_EQ(orderedElves[2].getNumCalories(), 10000);
    }

    TEST(Day1Answer, max_total_calories) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_1.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        CalorieCounter cc(str);

        auto maxElf = cc.getElfWithMaxCalories();
        std::cout << "[ DAY 1 ] Max Calories  = " << maxElf.getNumCalories() << std::endl;
    }

    TEST(Day1Answer, total_calories_top_3_elves) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_1.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        CalorieCounter cc(str);

        const auto orderedElves = cc.orderElvesByCalories();
        int calories = 0;
        for (int i = 0; i < 3; ++i) {
            calories += orderedElves[i].getNumCalories();
        }
        std::cout << "[ DAY 1 ] Calories for top 3 elves  = " << calories << std::endl;
    }

}
