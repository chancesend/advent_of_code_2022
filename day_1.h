//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_1_H
#define ADVENT_OF_CODE_2022_DAY_1_H

#include <string>
#include <sstream>
#include <vector>
#include <numeric>

using Calories = int;

class Elf {
public:
    using CalorieList = std::vector<Calories>;

    Elf() : _data() {}

    explicit Elf(CalorieList inData) : _data(inData)
    {
    }

    [[nodiscard]] int getNumCalories() const
    {
        auto sum = std::accumulate(_data.begin(), _data.end(), 0);
        return sum;
    }

private:
    CalorieList _data;
};

class CalorieCounter {
    using ElfList = std::vector<Elf>;

public:
    explicit CalorieCounter(const std::string& inData)
    {
        _elves = parseStringData(inData);
    }

    int getNumElves()
    {
        return static_cast<int>(_elves.size());
    }

    int getNumCaloriesForElf(int elfNum)
    {
        const auto elf = _elves[elfNum];
        return elf.getNumCalories();
    }

    Elf getElfWithMaxCalories()
    {
        auto maxElf = std::accumulate(_elves.begin(), _elves.end(), Elf(), [](Elf&& acc, const Elf& elf) {
            return (elf.getNumCalories() > acc.getNumCalories()) ? elf : acc;
        });
        return maxElf;
    }

    ElfList orderElvesByCalories()
    {
        ElfList sortedElves = _elves;
        std::sort(sortedElves.begin(), sortedElves.end(),
                                        [](const Elf & a, const Elf & b) -> bool
        {
            return a.getNumCalories() > b.getNumCalories();
        });
        return sortedElves;
    }

private:
    static ElfList parseStringData(const std::string& inData)
    {
        ElfList result;
        auto ss = std::stringstream{inData};

        Elf::CalorieList cl;
        for (std::string line; std::getline(ss, line, '\n');) {
            if (line.empty())
            {
                result.push_back(Elf(cl));
                cl.clear();
            }
            else
            {
                auto val = stoi(line);
                cl.push_back(val);
            }
        }
        if (!cl.empty())
        {
            result.push_back(Elf(cl));
        }

        return result;
    }

    std::string _data;
    ElfList     _elves;
};


#endif //ADVENT_OF_CODE_2022_DAY_1_H
