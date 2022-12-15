//
// Created by Ryan Avery on 12/14/2022.
//

#ifndef ADVENTOFCODE2022_DAY_14_H
#define ADVENTOFCODE2022_DAY_14_H

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
#include <Eigen/Dense>
#include <map>
#include <variant>
#include <memory>

namespace Day14 {

    using Grid = Eigen::ArrayXXi;

    enum Elem
    {
        Air     = 0,
        Rock    = 1,
        Sand    = 2
    };

class Pos
{
public:
    using List = std::vector<Pos>;
    Pos(const std::string& in)
    {
        auto [x, y] = parseInput(in);
        _x = x;
        _y = y;
    }
    Pos(int x, int y) : _x(x), _y(y) {}

    std::pair<int, int> get() const {return {_x, _y};}
private:
    std::pair<int, int> parseInput(const std::string& in)
    {
        std::string delimiter = ",";
        auto delPos = in.find(delimiter);
        auto xStr = in.substr(0, delPos);
        auto yStr = in.substr(delPos+1);
        auto x = stoi(xStr);
        auto y = stoi(yStr);
        return {x, y};
    }

    int _x;
    int _y;
};

class SandSimulator
{
public:
    SandSimulator(const std::string& in) : _grid(parseDataInput(in)) {}

    int getNumSandUntilSourceBlocked(Pos start)
    {
        // First construct the floor
        _grid.conservativeResize(Eigen::NoChange, _grid.cols() + 2);
        // This is confusing because our array representation is transposed
        _grid.rightCols(2) = Air;
        _grid.rightCols(1) = Rock;

        std::cout << "Start grid: " << std::endl;
        std::cout << _grid.transpose();

        int num = 0;
        while(doSandFall(start))
        {
            num++;
        }
        int count = (_grid == Sand).any();
        std::cout << "Final grid: " << std::endl;
        std::cout << _grid.transpose();
        return num;
    }

    int getNumSandBeforeFlowingIntoAbyss(Pos start)
    {
        int num = 0;
        while(doSandFall(start))
        {
            num++;
        }
        int count = (_grid == Sand).any();
        std::cout << "Final grid: " << std::endl;
        std::cout << _grid.transpose();
        return num;
    }

    bool doSandFall(Pos start) {
        auto [x, y] = start.get();
        if (_grid(x, y) != Air)
        {
            // We're blocked up now
            return false;
        }
        while (y+1 < _grid.cols())  // Once we're at the edge, we'll fall
        {
            assert(x+1 >= 0);
            if (x+1 >= _grid.rows())
            {
                _grid.conservativeResize(_grid.rows()+1, Eigen::NoChange);
                _grid.bottomRows(1) = Air;
                _grid.bottomRightCorner(1, 1) = Rock;
                assert(x+1 < _grid.rows());
            }
            assert(x-1 >= 0);
            assert(x-1 < _grid.rows());
            assert(y+1 >= 0);
            assert(y+1 < _grid.cols());

            auto elem = _grid(x, y);
            auto down = _grid(x, y+1);
            auto downl = _grid(x-1, y+1);
            auto downr = _grid(x+1, y+1);
            if (_grid(x, y+1) == Air)
            {
                y++;
            }
            else if (_grid(x-1, y+1) == Air)
            {
                y++;
                x--;
            }
            else if (_grid(x+1, y+1) == Air)
            {
                y++;
                x++;
            }
            else
            {
                _grid(x, y) = Sand;
                return true;
            }
        }
        return false;
    }

private:
    using RockConstructionLists = std::vector<Pos::List>;

    static Pos::List parseInputLine(std::string in)
    {
        std::string delimiter = "->";
        Pos::List pl;
        while(in.size())
        {
            auto delPos = in.find(delimiter);
            Pos pos(in.substr(0, delPos));
            pl.push_back(pos);
            if (delPos == std::string::npos)
                break;
            in.erase(0, delPos + delimiter.length());
        }
        return pl;
    }

    static void populateGrid(Grid& grid, RockConstructionLists& rcl)
    {
        for (const auto &movement: rcl) {
            auto [x, y] = movement[0].get();
            for (const auto &stop: movement)
            {
                const int nextX = stop.get().first;
                const int nextY = stop.get().second;
                const int startX = std::min(x, nextX);
                const int startY = std::min(y, nextY);
                grid.block(startX, startY, std::abs(nextX - x) + 1, std::abs(nextY - y) + 1) = Rock;
                std::tie(x, y) = stop.get();
            }
        }
    }

    static Grid parseDataInput(const std::string& in)
    {
        auto ss = std::stringstream{in};
        std::string line;

        RockConstructionLists pll;
        while(std::getline(ss, line, '\n'))
        {
            Pos::List pl(parseInputLine(line));
            pll.push_back(pl);
        }

        // Now construct the starting grid
        Grid grid = initializeGrid(pll);
        populateGrid(grid, pll);
        return grid;
    }

    static Grid initializeGrid(RockConstructionLists& pll)
    {
        Grid grid;
        auto xPosCmp = [](const Pos& l, const Pos& r){
            return l.get().first < r.get().first;
        };
        auto yPosCmp = [](const Pos& l, const Pos& r){
            return l.get().second < r.get().second;
        };

        int minX = pll[0][0].get().first;
        int minY = pll[0][0].get().second;
        int maxX = pll[0][0].get().first;
        int maxY = pll[0][0].get().second;
        for (auto& pl: pll)
        {
            auto [minXElem, maxXElem] = std::minmax_element(pl.begin(), pl.end(), xPosCmp);
            auto [minYElem, maxYElem] = std::minmax_element(pl.begin(), pl.end(), yPosCmp);
            minX = std::min(minX, minXElem->get().first);
            minY = std::min(minY, minYElem->get().second);
            maxX = std::max(maxX, maxXElem->get().first);
            maxY = std::max(maxY, maxYElem->get().second);
        }
        auto rows = maxX + 1;
        auto cols = maxY + 1;
        grid.resize(rows, cols);
        grid.setConstant(Air);
        return grid;
    }

    Grid _grid;
};

}

#endif //ADVENTOFCODE2022_DAY_14_H
