//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "day_13.h"

namespace Day13 {

    std::string testInput = R"([1,1,3,1,1]
[1,1,5,1,1]

[[1],[2,3,4]]
[[1],4]

[9]
[[8,7,6]]

[[4,4],4,4]
[[4,4],4,4,4]

[7,7,7,7]
[7,7,7]

[]
[3]

[[[]]]
[[]]

[1,[2,[3,[4,[5,6,7]]]],8,9]
[1,[2,[3,[4,[5,6,0]]]],8,9])";


    class TestDay13 : public ::testing::Test {
    public:
        TestDay13() : pf(testInput) {}

        PairFinder pf;
    };

    TEST_F(TestDay13, SumOfIndicesOfPairsInRightOrder) {
        auto indices = pf.indicesOfPairsInRightOrder();
        auto sum = std::accumulate(indices.begin(), indices.end(), 0);
        ASSERT_EQ(sum, 13);
    }

    TEST_F(TestDay13, GetDecoderKeyFromDividerPackets) {
        PacketDecoder pd(testInput);
        auto dividerPackets = pd.getDividerPackets();
        auto prod = dividerPackets[0] * dividerPackets[1];
        ASSERT_EQ(prod, 140);
    }

    TEST(Day13Answer1, SumIndicesOfPairsInRightOrder) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_13.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        PairFinder pf(str);
        auto indices = pf.indicesOfPairsInRightOrder();
        auto sum = std::accumulate(indices.begin(), indices.end(), 0);

        std::cout << "[ DAY 13 ] Sum of indices of pairs in right order  = " << sum << std::endl;
        ASSERT_EQ(sum, 5717);
    }

    TEST(Day13Answer2, ProductOfDividerPackets) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_13.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        PacketDecoder pd(str);
        auto dividerPackets = pd.getDividerPackets();
        auto prod = dividerPackets[0] * dividerPackets[1];

        std::cout << "[ DAY 13 ] Product of divider packets  = " << prod << std::endl;
        ASSERT_EQ(prod, 25935);
    }


}
