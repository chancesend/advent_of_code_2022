//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_6_H
#define ADVENT_OF_CODE_2022_DAY_6_H

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
        explicit DataStream(const std::string& in) : _stream(in) {
            constexpr int sopLookback = 4;
            constexpr int somLookback = 14;
            _sopMarker = calculateUniqueMarker(in, sopLookback);
            _somMarker = calculateUniqueMarker(in, somLookback);
        }

        [[nodiscard]] int getSopMarkerPos() const {return _sopMarker;}
        [[nodiscard]] int getSomMarkerPos() const {return _somMarker;}

    private:
        [[nodiscard]] static int calculateUniqueMarker(const std::string& in, int lookback)
        {
            for (auto subBegin = 0; subBegin + lookback != in.length(); subBegin++)
            {
                auto sub = in.substr(subBegin, lookback);
                std::sort(sub.begin(), sub.end());

                auto uniqueSet = std::accumulate(sub.begin(), sub.end(), std::string(""), [](std::string&& str, char chr){
                    if (str.back() != chr)
                        str += chr;
                    return str;
                });

                bool duplicateFound = (uniqueSet.size() != sub.size());
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

#endif //ADVENT_OF_CODE_2022_DAY_6_H
