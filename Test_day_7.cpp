//
// Created by Ryan Avery on 12/6/2022.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "day_7.h"

namespace Day7 {

    std::string testInput = R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k
)";

    class TestDay7 : public ::testing::Test {
    public:
        TestDay7() : db(testInput) {}

        DirectoryBrowser db;
    };

    TEST_F(TestDay7, SizeOfDirectories) {
        auto dirSizeList = db.getDirSizeList();

        ASSERT_EQ(getSizeOfDir(dirSizeList, "e"), 584);
        ASSERT_EQ(getSizeOfDir(dirSizeList, "a"), 94853);
        ASSERT_EQ(getSizeOfDir(dirSizeList, "d"), 24933642);
        ASSERT_EQ(db.getRoot().getSize(), 48381165);
    }

    TEST_F(TestDay7, SizesOfDirectoriesAtMostASize) {
        auto dirSizeList = db.getDirSizeList();
        ASSERT_EQ(getSizeOfDirectoriesAtMost(dirSizeList, 100000), 95437);
    }

    TEST_F(TestDay7, SizeOfDirectoryToDeleteToRunUpdate) {
        int totalDiskSpace = 70000000;
        int unusedSpaceNeeded = 30000000;
        auto unusedSpace = totalDiskSpace - db.getRoot().getSize();
        int expectedUnusedSpace = 21618835;
        ASSERT_EQ(unusedSpace, expectedUnusedSpace);

        int extraSpaceNeeded = unusedSpaceNeeded - unusedSpace;
        auto dirSizeList = db.getDirSizeList();
        ASSERT_EQ(getDirSizeToDeleteToReclaimSpace(dirSizeList, extraSpaceNeeded), 24933642);
    }

    TEST(Day7Answer1, SizesOfDirectoriesAtMost) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_7.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        DirectoryBrowser db(str);
        auto dirSizeList = db.getDirSizeList();
        int size = getSizeOfDirectoriesAtMost(dirSizeList, 100000);

        std::cout << "[ DAY 7 ] Total size of directories < 100000  = " << size << std::endl;
        ASSERT_EQ(size, 1501149);
    }

    TEST(Day7Answer2, SizeOfDirectoryToDeleteToRunUpdate) {
        const std::filesystem::path dir = INPUTS_DIR;
        std::ifstream inFile{dir / "input_day_7.txt"};
        ASSERT_TRUE(inFile);
        ASSERT_TRUE(inFile.is_open());

        std::ostringstream buffer;
        buffer << inFile.rdbuf();
        std::string str{buffer.str()};

        DirectoryBrowser db(str);
        int totalDiskSpace = 70000000;
        int unusedSpaceNeeded = 30000000;
        auto unusedSpace = totalDiskSpace - db.getRoot().getSize();

        int extraSpaceNeeded = unusedSpaceNeeded - unusedSpace;
        auto dirSizeList = db.getDirSizeList();
        int size = getDirSizeToDeleteToReclaimSpace(dirSizeList, extraSpaceNeeded);

        std::cout << "[ DAY 7 ] Total size of directories < 100000  = " << size << std::endl;
        ASSERT_EQ(size, 10096985);
    }

}
