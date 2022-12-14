//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_11.h"

namespace Day11 {

    std::string testInput = R"(Monkey 0:
  Starting items: 79, 98
  Operation: new = old * 19
  Test: divisible by 23
    If true: throw to monkey 2
    If false: throw to monkey 3

Monkey 1:
  Starting items: 54, 65, 75, 74
  Operation: new = old + 6
  Test: divisible by 19
    If true: throw to monkey 2
    If false: throw to monkey 0

Monkey 2:
  Starting items: 79, 60, 97
  Operation: new = old * old
  Test: divisible by 13
    If true: throw to monkey 1
    If false: throw to monkey 3

Monkey 3:
  Starting items: 74
  Operation: new = old + 3
  Test: divisible by 17
    If true: throw to monkey 0
    If false: throw to monkey 1)";


    class TestDay11 : public ::testing::Test {
    public:
        TestDay11() : mb(testInput) {}

        MonkeyBusiness mb;
    };

    TEST_F(TestDay11, MonkeyBusinessAfter20Rounds) {
        auto itemsSeen = mb.getNumItemsSeenAfterRounds(20);
        auto business = getMonkeyBusiness(itemsSeen);
        ASSERT_EQ(business, 10605);
    }

    TEST_F(TestDay11, MonkeyBusinessAfter10000RoundsIncreasedWorry) {
        MonkeyBusiness mbNoDiv(testInput, false);

        auto itemsSeen = mbNoDiv.getNumItemsSeenAfterRounds(10000);
        auto business = getMonkeyBusiness(itemsSeen);
        ASSERT_EQ(business, 2713310158);
    }

    TEST(Day11Answer1, MonkeyBusinessAfter20Rounds) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_11.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        MonkeyBusiness mb(str);
        auto itemsSeen = mb.getNumItemsSeenAfterRounds(20);
        auto business = getMonkeyBusiness(itemsSeen);

        std::cout << "[ DAY 11 ] Monkey business after 20 rounds  = " << business << std::endl;
        ASSERT_EQ(business, 102399);
    }

    TEST(Day11Answer2, MonkeyBusinessAfter10000RoundsIncreasedWorry) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_11.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        MonkeyBusiness mb(str, false);
        auto itemsSeen = mb.getNumItemsSeenAfterRounds(10000);
        auto business = getMonkeyBusiness(itemsSeen);

        std::cout << "[ DAY 11 ] Monkey business after 10000 rounds, increased worry  = " << business << std::endl;
        ASSERT_EQ(business, 23641658401);
    }

}
