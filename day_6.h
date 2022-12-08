//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_6_H
#define ADVENTOFCODE2022_DAY_6_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>

namespace Day6 {

    class DataStream {
    public:
        DataStream(std::string in) : _stream(in) {
            constexpr int sopLookback = 4;
            constexpr int somLookback = 14;
            _sopMarker = calculateUniqueMarker(in, sopLookback);
            _somMarker = calculateUniqueMarker(in, somLookback);
        }

        int getSopMarkerPos () {return _sopMarker;}
        int getSomMarkerPos () {return _somMarker;}

    private:
        int calculateUniqueMarker(std::string in, int lookback)
        {
            auto subBegin = 0;
            for (; subBegin + lookback != in.length(); subBegin++)
            {
                auto sub = in.substr(subBegin, lookback);
                std::sort(sub.begin(), sub.end());

                bool duplicateFound = false;
                for (auto subIter = sub.begin(); subIter != sub.end() - 1; subIter++)
                {
                    if (*subIter == *(subIter+1))
                    {
                        duplicateFound = true;
                    }
                }
                if (!duplicateFound)
                {
                    // All characters are different
                    auto sopMarker = subBegin + lookback;
                    return sopMarker;
                }
            }
            return -1;
        }

        std::string _stream;
        int _sopMarker;
        int _somMarker;
    };

}

#endif //ADVENTOFCODE2022_DAY_6_H
