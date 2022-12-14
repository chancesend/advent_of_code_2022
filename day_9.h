//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_9_H
#define ADVENT_OF_CODE_2022_DAY_9_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>
#include <cassert>

namespace Day9 {

    using Pos = std::pair<int, int>;

    class Move
    {
    public:
        using List = std::vector<Move>;
        using Dir = char;

        explicit Move(const std::string& in) : _steps(0) {
            std::tie(_relDir, _steps) = parseStringData(in);
        }
        Move(Dir dir, int steps) : _relDir(dirToRelDir(dir)), _steps(steps) {}

        Pos relDir() {return _relDir;}
        [[nodiscard]] int steps() const {return _steps;}

    private:
        static Pos dirToRelDir(char dir)
        {
            Pos relDir;
            switch(dir)
            {
                case 'U':
                    relDir = {0, 1};
                    break;
                case 'D':
                    relDir = {0, -1};
                    break;
                case 'L':
                    relDir = {-1, 0};
                    break;
                case 'R':
                    relDir = {1, 0};
                    break;
                default:
                    assert(!"Unknown dir");
            }
            return relDir;
        }

        static std::pair<Pos, int> parseStringData(const std::string& in)
        {
            auto relPos = dirToRelDir(in.substr(0, 1)[0]);
            int steps = std::stoi(in.substr(2));
            return {relPos, steps};
        }

        Pos _relDir;
        int _steps;
    };

    class Board
    {
    public:
        using Dims = std::pair<int, int>;
        using Vec = std::vector<int>;
        using Matrix = std::vector<Vec>;

        Board() : Board(Dims(0,0)) {}

        explicit Board(Dims dims)
        {
            resize(dims);
            _startingPos = Pos((dims.first-1)/2, (dims.second-1)/2);
            _board[_startingPos.first][_startingPos.second] = 1;   // Tail starts at origin
        }

        Pos startingPos()
        {
            return _startingPos;
        }

        [[nodiscard]] Dims dims() const
        {
            Dims dims{_board.size(), _board[0].size()};
            return dims;
        }
        [[nodiscard]] int posVal(Pos pos) const
        {
            return _board[pos.first][pos.second];
        }

        void resize(Dims dims)
        {
            _board.resize(dims.first);
            for(int i = 0; i < dims.first; ++i)
            {
                _board[i].resize(dims.second);
            }
        }



        Pos doTailMove(Pos knot, Pos head, bool isTail)
        {
            auto dist = [](int p1, int p2)
            {
                return abs(p1 - p2);
            };
            auto sign = [](int p1, int p2)
            {
                return (p1 - p2) > 0 ? 1 : ((p1 - p2) < 0 ? -1 : 0);
            };

            while ((dist(head.first, knot.first) > 1) || (dist(head.second, knot.second) > 1))
            {
                knot.first += sign(head.first, knot.first);
                knot.second += sign(head.second, knot.second);
                if (isTail)
                {
                    // Record tail visit
                    _board[knot.first][knot.second] = 1;
                }
            }

            return knot;
        }

        Pos doHeadMove(Pos start, Pos relDir)
        {
            auto boardDim = dims();
            assert(start.first < boardDim.first);
            assert(start.second < boardDim.second);

            Pos newPos = start;
            newPos.first += relDir.first;
            newPos.second += relDir.second;

            assert(newPos.first >= 0);
            assert(newPos.second >= 0);

            {
                // Resize the board if necessary
                Dims boardSize = {_board.size(), _board[0].size()};
                boardSize.first = std::max(boardSize.first, newPos.first + 1);
                boardSize.second = std::max(boardSize.second, newPos.second + 1);
                resize(boardSize);
            }

            {
                auto newBoardDim = dims();
                assert(newPos.first < newBoardDim.first);
                assert(newPos.second < newBoardDim.second);
            }

            return newPos;
        }

    private:
        Matrix _board;
        Pos _startingPos;
    };

    class RopeMover {
    public:
        RopeMover(const std::string& in, int numKnots, Board::Dims initialSize = {1, 1}) :
            _movesList(parseStringData(in)),
            _board(initialSize),
            _knots(numKnots)
        {
            _startingPos = _board.startingPos();
            for(auto& knot: _knots)
            {
                knot = _startingPos;
            }
            doMoves(_movesList);
        }

        [[nodiscard]] int numPlacesTailVisited() const
        {
            // Refactor the board so that we can use some stdlib accumulation to do this
            int sum = 0;
            for (int i = 0; i < _board.dims().first; ++i)
            {
                for (int j = 0; j < _board.dims().second; ++j)
                {
                    sum += _board.posVal({i, j});
                }
            }
            return sum;
        }

    private:
        using Knots = std::vector<Pos>;

        void doRopeMove(Move move)
        {
            for (int moves = 0; moves < move.steps(); ++moves)
            {
                _knots[0] = _board.doHeadMove(_knots[0], move.relDir());
                for (int i = 1; i < _knots.size() - 1; ++i)
                {
                    bool isTail = false;
                    _knots[i] = _board.doTailMove(_knots[i], _knots[i-1], isTail);
                }
                // Do tail knot, updating the tail pos
                _knots[_knots.size()-1] = _board.doTailMove(_knots[_knots.size()-1], _knots[_knots.size()-2], true);
            }
        }

        void doMoves(const Move::List& ml)
        {
            for(const auto& move: ml)
            {
                doRopeMove(move);
            }
        }

        static Move::List parseStringData(const std::string& in)
        {
            Move::List ml;
            auto ss = std::stringstream{in};

            for (std::string line; std::getline(ss, line, '\n');) {
                Move move(line);
                ml.push_back(move);
            }

            return ml;
        }

        Move::List _movesList;
        Board _board;

        Pos _startingPos;
        Knots       _knots;
    };

}

#endif //ADVENT_OF_CODE_2022_DAY_9_H
