//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_12.h"

namespace Day12 {

    std::string testInput = R"(Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi)";


    class TestDay12 : public ::testing::Test {
    public:
        TestDay12() : pf(testInput) {}

        PathFinder pf;
    };

    TEST_F(TestDay12, ShortestPath) {
        auto shortestPath = pf.findShortestPathFromStart();
        ASSERT_EQ(shortestPath, 31);
    }

    TEST_F(TestDay12, ShortestPathStartingAtAnyA) {
        auto shortestPath = pf.findShortestPathFromAny('a');
        ASSERT_EQ(shortestPath, 29);
    }


    TEST(Day12Answer1, ShortestPath) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_12.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        PathFinder pf(str);
        auto shortestPath = pf.findShortestPathFromStart();

        std::cout << "[ DAY 12 ] Shortest path  = " << shortestPath << std::endl;
        ASSERT_EQ(shortestPath, 383);
    }


    TEST(Day12Answer2, ShortestPath) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_12.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        PathFinder pf(str);
        auto shortestPath = pf.findShortestPathFromAny('a');

        std::cout << "[ DAY 12 ] Shortest path from any A  = " << shortestPath << std::endl;
        ASSERT_EQ(shortestPath, 377);
    }


}
