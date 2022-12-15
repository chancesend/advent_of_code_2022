//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_12_H
#define ADVENT_OF_CODE_2022_DAY_12_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>
#include <cassert>
#include <deque>
#include "Eigen/Dense"
#include <map>

namespace Day12 {
    using Map = Eigen::ArrayXXi;
    using Elevation = int;
    using Pos = std::pair<int, int>;

    Pos operator+(const Pos& l, const Pos& r)
    {
        return {l.first + r.first, l.second + r.second};
    }

    bool inBounds(Map map, Pos pos)
    {
        bool rowInBounds = (pos.first >= 0) && (pos.first < map.rows());
        bool colInBounds = (pos.second >= 0) && (pos.second < map.cols());
        return rowInBounds && colInBounds;
    }

    class PathFinder
    {
    public:
        std::vector<Pos> dirs = {
                {0, -1},
                {0, 1},
                {1, 0},
                {-1, 0},
        };

        PathFinder(const std::string& in)
        {
            std::tie(_map, _start, _end) = parseDataInput(in);
        }

        bool canStep(Map map, Pos cur, Pos next)
        {
            return map(next.first, next.second) - map(cur.first, cur.second) <= 1;
        }
        bool hasStepped(std::vector<Pos> history, Pos cur)
        {
            return std::find(history.begin(), history.end(), cur) != history.end();
        }

        using PosSteps = std::pair<Pos, int>;
        using PosStepsQueue = std::deque<PosSteps>;
        using PosStepsVector = std::vector<PosSteps>;

        template <typename Iter>
        bool hasStepped(const Iter& begin, const Iter& end, Pos cur)
        {
            auto found = std::find_if(begin, end, [cur](const PosSteps& ps){
                return ps.first == cur;
            });
            return found != end;
        }

        int findShortestPathFromStart()
        {
            PosStepsQueue queue;
            PosStepsVector visited;
            queue.push_back({_start, 0});
            int minSteps = 0;
            doPathStep(queue, visited, minSteps);
            return minSteps;
        }

        bool doPathStep(PosStepsQueue& queue, PosStepsVector& visited, int& numSteps)
        {
            while(!queue.empty())
            {
                const auto posAndStepNum = queue.front();
                const auto pos = posAndStepNum.first;
                const auto stepNum = posAndStepNum.second;
                queue.pop_front();

                if (pos == _end) {
                    numSteps = stepNum;
                    return true;
                }

                visited.push_back(posAndStepNum);
                for(auto& dir: dirs)
                {
                    auto newPos = pos + dir;
                    if (!inBounds(_map, newPos)) {
                        continue;
                    }
                    if (!canStep(_map, pos, newPos)) {
                        continue;
                    }
                    if (hasStepped(visited.begin(), visited.end(), newPos)) {
                        continue;
                    }
                    if (hasStepped(queue.begin(), queue.end(), newPos)) {
                        continue;
                    }
                    queue.push_back({newPos, stepNum + 1});
                }
            }

            return false;
        }

        int findShortestPathFromAny(char startLevel)
        {
            int start = startLevel - 'a';

            std::vector<Pos> possibleStarts;
            int trueShortestPath = _map.size();
            // TODO: Would be nice to use Array::visit, but that requires a bit of glue to make work
            PosStepsQueue queue;
            PosStepsVector visited;
            for (int i = 0; i < _map.rows(); ++i)
            {
                for (int j = 0; j < _map.cols(); ++j)
                {
                    Pos pos(i, j);
                    if (_map(i, j) == start)
                    {
                        queue.push_back({pos, 0});
                        int shortestPath;
                        auto found = doPathStep(queue, visited, shortestPath);
                        if (found)
                            trueShortestPath = std::min(trueShortestPath, shortestPath);
                    }
                    queue.clear();
                    visited.clear();
                }
            }

            return trueShortestPath;
        }

    private:
        std::tuple<Map, Pos, Pos> parseDataInput(const std::string& in)
        {
            auto ss = std::stringstream{in};
            std::string line;

            auto [rows, cols] = getDataDims(in);
            Map map(rows, cols);

            int row = 0;
            Pos start;
            Pos end;
            while(std::getline(ss, line, '\n'))
            {
                for (int col = 0; col < line.size(); ++col)
                {
                    Elevation elev = line[col] - 'a';
                    if (line[col] == 'S') {
                        elev = 0;
                        start = {row, col};
                    }
                    else if (line[col] == 'E') {
                        elev = 'z' - 'a';
                        end = {row, col};
                    }
                    map(row, col) = elev;
                }
                row++;
            }

            return {map, start, end};
        }

        std::pair<int, int> getDataDims(const std::string& in)
        {
            auto ss = std::stringstream{in};
            std::string line;
            int rows = 0;
            int cols = 0;
            while(std::getline(ss, line, '\n'))
            {
                rows++;
                cols = line.size();
            }
            return {rows, cols};
        }

        Map _map;
        Pos _start;
        Pos _end;
    };

}

#endif //ADVENT_OF_CODE_2022_DAY_12_H
