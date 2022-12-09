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
        using Vec = std::vector<int>;
        using Dims = std::pair<int, int>;
        using Map = std::vector<Vec>;

        TreeMap() : _map() {}
        TreeMap(Dims dims) : _map(dims.first) {
            for(auto& row: _map)
            {
                row.resize(dims.second);
            }
        }

        Vec row(int i) const
        {
            return _map[i];
        }

        void setRow(int i, Vec vec)
        {
            _map[i] = vec;
        }

        Vec col(int j) const
        {
            Vec col;
            col.reserve(_map[0].size());
            col = std::accumulate(_map.cbegin(), _map.cend(), col, [j](Vec&& acc, const Vec& vec){
                acc.push_back(vec[j]);
                return std::move(acc);
            });
            return col;
        }

        void setCol(int j, Vec vec)
        {
            for(int i = 0; i < _map[0].size(); ++i)
            {
                _map[i][j] = vec[i];
            }
        }

        void push_back(Vec row)
        {
            _map.push_back(row);
        }

        Dims getDims() const {return {_map.size(), _map[0].size()};}

        Map _map;
    };

    void updateVisibleMap(TreeMap& map, const TreeMap& dir)
    {
        TreeMap::Dims dims = map.getDims();
        for (int i = 0; i < dims.first; ++i)
        {
            auto mapRow = map.row(i);
            auto dirRow = dir.row(i);
            TreeMap::Vec totalVisible;
            std::transform(mapRow.cbegin(), mapRow.cend(), dirRow.cbegin(), mapRow.begin(), [](int l, int r)
            {
                // Return true if tree is visible from any angle
                return ((l > 0) || (r >= 0));
            });
            map.setRow(i, mapRow);
        }
    }

    int sumNumVisible(const TreeMap& map)
    {
        int sum = 0;
        for (int i = 0; i < map.getDims().first; ++i)
        {
            auto mapRow = map.row(i);
            auto thisSum = std::accumulate(mapRow.cbegin(), mapRow.cend(), 0, [](int acc, int val) -> int {
                return acc + val;
            });
            sum += thisSum;
        }

        return sum;
    }

    class TreeViewer
    {
    public:

        TreeViewer(std::string in) :
            _treemap(parseStringData(in)),
            _treemapDims(_treemap.getDims())
        {
        }

        int numVisibleTrees() const
        {
            // Look at trees from all 4 directions to calculate the visibleMatrix
            TreeMap visibleMap(_treemapDims);

            // North
            {
                TreeMap visibleThisDir(_treemapDims);
                TreeMap::Vec max = _treemap.row(0);
                visibleThisDir.setRow(0, max); // All trees on edge are visible
                for (int i = 1; i < _treemapDims.first; ++i)
                {
                    TreeMap::Vec vec = _treemap.row(i);
                    TreeMap::Vec visibleResult;
                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), std::back_inserter(visibleResult), [](int l, int r)
                    {
                        return l > r ? l : -1;
                    });
                    visibleThisDir.setRow(i, visibleResult);

                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), max.begin(), [](int l, int r)
                    {
                        return std::max(l, r);
                    });
                }
                updateVisibleMap(visibleMap, visibleThisDir);
            }
            // South
            {
                TreeMap visibleThisDir(_treemapDims);
                TreeMap::Vec max = _treemap.row(_treemapDims.first - 1); // All trees on edge are visible
                visibleThisDir.setRow(_treemapDims.first - 1, max);
                for (int i = _treemapDims.first - 2; i >= 0; --i)
                {
                    TreeMap::Vec vec = _treemap.row(i);
                    TreeMap::Vec visibleResult;
                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), std::back_inserter(visibleResult), [&](int l, int r)
                    {
                        return l > r ? l : -1;
                    });
                    visibleThisDir.setRow(i, visibleResult);

                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), max.begin(), [](int l, int r)
                    {
                        return std::max(l, r);
                    });
                }
                updateVisibleMap(visibleMap, visibleThisDir);
            }
            // West
            {
                TreeMap visibleThisDir(_treemapDims);
                TreeMap::Vec max = _treemap.col(0); // All trees on edge are visible
                visibleThisDir.setCol(0, max);
                for (int i = 1; i < _treemapDims.second; ++i)
                {
                    TreeMap::Vec vec = _treemap.col(i);
                    TreeMap::Vec visibleResult;
                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), std::back_inserter(visibleResult), [&](int l, int r)
                    {
                        return l > r ? l : -1;
                    });
                    visibleThisDir.setCol(i, visibleResult);

                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), max.begin(), [](int l, int r)
                    {
                        return std::max(l, r);
                    });
                }
                updateVisibleMap(visibleMap, visibleThisDir);
            }
            // East
            {
                TreeMap visibleThisDir(_treemapDims);
                TreeMap::Vec max = _treemap.col(_treemapDims.second - 1); // All trees on edge are visible
                visibleThisDir.setCol(_treemapDims.second - 1, max);
                for (int i = _treemapDims.second - 2; i >= 0; --i)
                {
                    TreeMap::Vec vec = _treemap.col(i);
                    TreeMap::Vec visibleResult;
                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), std::back_inserter(visibleResult), [&](int l, int r)
                    {
                        return l > r ? l : -1;
                    });
                    visibleThisDir.setCol(i, visibleResult);

                    std::transform(vec.cbegin(), vec.cend(), max.cbegin(), max.begin(), [](int l, int r)
                    {
                        return std::max(l, r);
                    });
                }
                updateVisibleMap(visibleMap, visibleThisDir);
            }

            int numVisible = sumNumVisible(visibleMap);
            return numVisible;
        }

        int getScenicScore(TreeMap::Dims coords)
        {
            const auto [row, col] = coords;
            auto targetHeight = _treemap.row(row)[col];
            auto scenicScore = 1;
            // North
            {
                int viewingDistance = 0;
                for (int i = row - 1; i >= 0; --i) {
                    viewingDistance++;
                    auto consideredTree = _treemap.row(i)[col];
                    if (consideredTree >= targetHeight)
                        break;
                }
                scenicScore *= viewingDistance;
            }
            // South
            {
                int viewingDistance = 0;
                for (int i = row + 1; i < _treemap.getDims().first; ++i) {
                    viewingDistance++;
                    auto consideredTree = _treemap.row(i)[col];
                    if (consideredTree >= targetHeight)
                        break;
                }
                scenicScore *= viewingDistance;
            }
            // East
            {
                int viewingDistance = 0;
                for (int i = col + 1; i < _treemap.getDims().second; ++i) {
                    viewingDistance++;
                    auto consideredTree = _treemap.row(row)[i];
                    if (consideredTree >= targetHeight)
                        break;
                }
                scenicScore *= viewingDistance;
            }
            // West
            {
                int viewingDistance = 0;
                for (int i = col -1; i >= 0; --i) {
                    viewingDistance++;
                    auto consideredTree = _treemap.row(row)[i];
                    if (consideredTree >= targetHeight)
                        break;
                }
                scenicScore *= viewingDistance;
            }
            return scenicScore;
        }

        int getHighestScenicScore()
        {
            auto dims = _treemap.getDims();
            auto highestScore = 0;
            for (int i = 0; i < dims.first; ++i)
            {
                for (int j = 0; j < dims.second; ++j)
                {
                    auto thisScore = getScenicScore({i, j});
                    highestScore = std::max(highestScore, thisScore);
                }
            }
            return highestScore;
        }

    private:
        TreeMap parseStringData(std::string inData)
        {
            TreeMap map;
            auto ss = std::stringstream{inData};

            for (std::string line; std::getline(ss, line, '\n');) {
                TreeMap::Vec row;
                for(auto chr: line)
                {
                    int height = stoi(std::string(1, chr));
                    row.push_back(height);
                }
                map.push_back(row);
            }

            return map;
        }

        TreeMap _treemap;
        TreeMap::Dims _treemapDims;
    };

}

#endif //ADVENTOFCODE2022_DAY_8_H
