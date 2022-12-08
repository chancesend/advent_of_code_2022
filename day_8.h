//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_8_H
#define ADVENTOFCODE2022_DAY_8_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>

namespace Day8 {

    class TreeMap
    {
    public:
        TreeMap(std::string in) : _treemap(parseStringData(in))
        {

        }

        int numVisibleTrees()
        {
            int numVisible = 0;
        }

    private:
        using Height = int;
        using TreeRow = std::vector<Height>;
        using Map = std::vector<TreeRow>;
        Map parseStringData(std::string inData)
        {
            Map map;
            auto ss = std::stringstream{inData};

            for (std::string line; std::getline(ss, line, '\n');) {
                TreeRow row;
                for(auto chr: line)
                {
                    int height = stoi(std::string(1, chr));
                    row.push_back(height);
                }
                map.push_back(row);
            }

            return list;
        }

        Map _treemap;
    };
}

#endif //ADVENTOFCODE2022_DAY_8_H
