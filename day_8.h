//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_8_H
#define ADVENT_OF_CODE_2022_DAY_8_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>
#include <Eigen/Dense>

namespace Day8 {
    using TreeMap = Eigen::ArrayXXi;
    using Vec = Eigen::Array<int, 1, Eigen::Dynamic>;
    using Dims = std::pair<int, int>;

    Dims getDims(const TreeMap& map)
    {
        return {map.rows(), map.cols()};
    }

    int sumNumVisible(const TreeMap& map)
    {
        return map.sum();
    }

    template<typename T, int R, int C>
    static std::string toString(const Eigen::Array<T,R,C>& mat){
        std::stringstream ss;
        ss << mat;
        return ss.str();
    }

    class TreeViewer
    {
    public:

        explicit TreeViewer(const std::string& in) :
            _treemap(parseStringData(in)),
            _treemapDims({_treemap.rows(), _treemap.cols()})
        {
        }

        [[nodiscard]] int numVisibleTrees()
        {
            // Look at trees from all 4 directions to calculate the visible array
            TreeMap visibleMap(_treemapDims.first, _treemapDims.second);
            visibleMap.setZero();

            auto visibilityFcn = [](int l, int r)
            {
                return l > r ? l : -1;
            };

            auto visibleMapUpdateFcn = [](int l, int r)
            {
                // Return true if tree is visible from any angle
                return ((l > 0) || (r >= 0)) ? 1 : 0;
            };

            // Ugh, I'd really like to refactor this into reusable sections,
            // probably by making the matrix rotatable

            // North
            {
                TreeMap visibleThisDir(_treemapDims.first, _treemapDims.second);
                Vec max = _treemap.row(0);
                visibleThisDir.row(0) = max; // All trees on edge are visible
                for (int i = 1; i < _treemapDims.first; ++i)
                {
                    Vec vec = _treemap.row(i);
                    Vec temp = vec.binaryExpr(max, visibilityFcn);
                    visibleThisDir.row(i) = temp;
                    max = vec.cwiseMax(max);
                }
                visibleMap = visibleMap.binaryExpr(visibleThisDir, visibleMapUpdateFcn);
                auto strVis = toString(visibleThisDir);
                auto strMap = toString(visibleMap);
            }
            // South
            {
                TreeMap visibleThisDir(_treemapDims.first, _treemapDims.second);
                Vec max = _treemap.row(_treemapDims.first - 1); // All trees on edge are visible
                visibleThisDir.row(_treemapDims.first - 1) = max;
                for (int i = _treemapDims.first - 2; i >= 0; --i)
                {
                    Vec vec = _treemap.row(i);
                    Vec temp = vec.binaryExpr(max, visibilityFcn);
                    visibleThisDir.row(i) = temp;
                    max = vec.cwiseMax(max);
                }
                visibleMap = visibleMap.binaryExpr(visibleThisDir, visibleMapUpdateFcn);
                auto strVis = toString(visibleThisDir);
                auto strMap = toString(visibleMap.eval());
            }
            // West
            {
                TreeMap visibleThisDir(_treemapDims.first, _treemapDims.second);
                Vec max = _treemap.col(0);
                visibleThisDir.col(0) = max; // All trees on edge are visible
                for (int i = 1; i < _treemapDims.second; ++i)
                {
                    Vec vec = _treemap.col(i);
                    Vec temp = vec.binaryExpr(max, visibilityFcn);
                    visibleThisDir.col(i) = temp;
                    max = vec.cwiseMax(max);
                }
                visibleMap = visibleMap.binaryExpr(visibleThisDir, visibleMapUpdateFcn);
                auto strVis = toString(visibleThisDir);
                auto strMap = toString(visibleMap.eval());
            }
            // East
            {
                TreeMap visibleThisDir(_treemapDims.first, _treemapDims.second);
                Vec max = _treemap.col(_treemapDims.second - 1); // All trees on edge are visible
                visibleThisDir.col(_treemapDims.second - 1) = max;
                for (int i = _treemapDims.second - 2; i >= 0; --i)
                {
                    Vec vec = _treemap.col(i);
                    Vec temp = vec.binaryExpr(max, visibilityFcn);
                    visibleThisDir.col(i) = temp;
                    max = vec.cwiseMax(max);
                }
                visibleMap = visibleMap.binaryExpr(visibleThisDir, visibleMapUpdateFcn);
                auto strVis = toString(visibleThisDir);
                auto strMap = toString(visibleMap.eval());
            }

            int numVisible = sumNumVisible(visibleMap.eval());
            auto visStr = toString(visibleMap);
            _visibleMap = visibleMap;
            return numVisible;
        }

        int getScenicScore(Dims coords)
        {
            const auto [row, col] = coords;
            int targetHeight = _treemap(row, col);
            auto scenicScore = 1;
            // North
            {
                int viewingDistance = 0;
                for (int i = row - 1; i >= 0; --i) {
                    viewingDistance++;
                    int consideredTree = _treemap(i, col);
                    if (consideredTree >= targetHeight)
                        break;
                }
                scenicScore *= viewingDistance;
            }
            // South
            {
                int viewingDistance = 0;
                for (int i = row + 1; i < _treemap.rows(); ++i) {
                    viewingDistance++;
                    int consideredTree = _treemap(i, col);
                    if (consideredTree >= targetHeight)
                        break;
                }
                scenicScore *= viewingDistance;
            }
            // East
            {
                int viewingDistance = 0;
                for (int i = col + 1; i < _treemap.cols(); ++i) {
                    viewingDistance++;
                    int consideredTree = _treemap(row, i);
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
                    int consideredTree = _treemap(row, i);
                    if (consideredTree >= targetHeight)
                        break;
                }
                scenicScore *= viewingDistance;
            }
            return scenicScore;
        }

        int getHighestScenicScore()
        {
            auto highestScore = 0;
            auto dims = getDims(_treemap);
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
        static TreeMap parseStringData(const std::string& inData)
        {
            auto ss = std::stringstream{inData};

            auto [rows, cols] = getArrayDimsFromInput(inData);
            TreeMap map(rows, cols);

            int row = 0;
            for (std::string line; std::getline(ss, line, '\n');) {
                auto thisCols = line.length();
                Vec col(thisCols);
                int thisCol = 0;
                for(auto chr: line)
                {
                    int height = stoi(std::string(1, chr));
                    col(thisCol) = height;
                    thisCol++;
                }

                map.row(row) = col;
                row++;
                auto mapStr = toString(map);
            }
            auto str = toString(map);

            return map;
        }

        static std::pair<int, int> getArrayDimsFromInput(const std::string& inData) {
            auto ss = std::stringstream{inData};
            int rows = 0;
            int cols = 0;
            for (std::string line; std::getline(ss, line, '\n');) {
                cols = static_cast<int>(line.length());
                Vec col(cols);
                rows++;
            }

            return {rows, cols};
        }

        TreeMap _treemap;
        TreeMap _visibleMap;
        Dims _treemapDims;
    };
}

#endif //ADVENT_OF_CODE_2022_DAY_8_H
