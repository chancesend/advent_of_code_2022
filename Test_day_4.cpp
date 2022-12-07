//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_4.h"

namespace Day4 {

    std::string test_data = R"(2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8
)";

    class TestDay4 : public ::testing::Test {
    public:
        TestDay4() : sa(test_data) {}

        SectionAssignments sa;
    };

    TEST_F(TestDay4, NumAssignmentPairsFullyContained) {
        ASSERT_EQ(sa.numPairsFullyContained(), 2);
    }

    TEST_F(TestDay4, NumAssignmentPairsOverlapping) {
        ASSERT_EQ(sa.numPairsOverlapping(), 4);
    }

    TEST_F(TestDay4, SectionAssignmentInput) {
        {
            SectionAssignment sa("2-4,6-8");
            ASSERT_EQ(sa.isFullyContained(), false);
            ASSERT_EQ(sa.isOverlapping(), false);
        }
        {
            SectionAssignment sa("2-8,3-7");
            ASSERT_EQ(sa.isFullyContained(), true);
            ASSERT_EQ(sa.isOverlapping(), true);
        }
        {
            SectionAssignment sa("2-6,4-8");
            ASSERT_EQ(sa.isFullyContained(), false);
            ASSERT_EQ(sa.isOverlapping(), true);
        }
    }

    TEST(Day4Answer1, NumAssignmentPairsFullyContained) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_4.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        SectionAssignments sa(str);

        auto num = sa.numPairsFullyContained();

        std::cout << "[ DAY 4 ] Num pairs fully contained  = " << num << std::endl;
    }

    TEST(Day4Answer2, NumAssignmentPairsOverlapping) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_4.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};
        SectionAssignments sa(str);

        auto num = sa.numPairsOverlapping();

        std::cout << "[ DAY 4 ] Num pairs overlapping  = " << num << std::endl;
    }
}
