//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_3.h"

namespace Day3 {

    std::string test_data = R"(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw
)";

    class TestDay3 : public ::testing::Test {
    public:
        TestDay3() : rs(test_data) {}

        Rucksacks rs;
    };

    TEST_F(TestDay3, PriorityOfItem) {
        ASSERT_EQ(Item('p').priority(), 16);
        ASSERT_EQ(Item('L').priority(), 38);
    }

    TEST_F(TestDay3, ItemInBothCompartments) {
        auto commonItem = Rucksack("vJrwpWtwJgWrhcsFMMfFFhFp").itemInBothCompartments();
        ASSERT_EQ(commonItem.get(), 'p');
    }

    TEST_F(TestDay3, SumOfPrioritiesOfItemTypeInBothRucksacks) {
        auto itemList = rs.commonItems();
        auto sum = sumOfPriorities(itemList);
        ASSERT_EQ(sum, 157);
    }

    TEST_F(TestDay3, GetGroupBadge)
    {
        {
            std::string test_data = R"(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg)";

            Rucksacks rs(test_data);
            auto eg = rs.getElfGroups();
            ASSERT_EQ(eg[0].getBadge().get(), 'r');
        }
        {
            std::string test_data = R"(wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw)";

            Rucksacks rs(test_data);
            auto eg = rs.getElfGroups();
            ASSERT_EQ(eg[0].getBadge().get(), 'Z');
        }
    }

    TEST_F(TestDay3, GetSumOfGroupBadges) {
        auto elfGroups = rs.getElfGroups();
        auto sum = sumOfGroupBadges(elfGroups);
        ASSERT_EQ(sum, 70);
    }

    TEST(Day3Answer1, SumOfPrioritiesOfItemTypeInBothRucksacks) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_3.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        Rucksacks rs(str);

        auto itemList = rs.commonItems();
        auto sum = sumOfPriorities(itemList);

        std::cout << "[ DAY 3 ] Sum of priorities  = " << sum << std::endl;
        ASSERT_EQ(sum, 7826);
    }


    TEST(Day3Answer2, SumOfPrioritiesOfElfGroupBadges) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_3.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        Rucksacks rs(str);

        auto elfGroups = rs.getElfGroups();
        auto sum = sumOfGroupBadges(elfGroups);

        std::cout << "[ DAY 3 ] Sum of elf group badges = " << sum << std::endl;
        ASSERT_EQ(sum, 2577);
    }
}
