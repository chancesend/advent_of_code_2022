//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_5_H
#define ADVENT_OF_CODE_2022_DAY_5_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>

namespace Day5 {

    struct Move {
        enum Type {
            CM_9000 = 9000,
            CM_9001 = 9001
        };

        int numToMove;
        int fromStack;
        int toStack;
    };

    class CrateStacks {
    public:
        using Crate = char;
        using Stack = std::stack<Crate>;
        using List = std::vector<Stack>;

        CrateStacks() = default;

        explicit CrateStacks(const std::string& in) : _stacks(parseStringData(in)) {}

        [[nodiscard]] Stack getStack(int i) const { return _stacks[i]; }

        [[nodiscard]] std::string getTopOfStacks() const {
            auto tosStr = std::accumulate(_stacks.begin(), _stacks.end(), std::string(""), [](std::string&& str, const Stack& stack) {
                str += stack.top();
                return str;
            });
            return tosStr;
        }

        void doMove(Move move, Move::Type type) {
            auto &fromStack = _stacks[move.fromStack];
            auto &toStack = _stacks[move.toStack];

            if (type == Move::Type::CM_9000) {
                for (int i = 0; i < move.numToMove; ++i) {
                    const auto val = fromStack.top();
                    fromStack.pop();
                    toStack.push(val);
                }
            } else if (type == Move::Type::CM_9001) {
                Stack tempStack;
                for (int i = 0; i < move.numToMove; ++i) {
                    tempStack.push(fromStack.top());
                    fromStack.pop();
                }
                for (int i = 0; i < move.numToMove; ++i) {
                    toStack.push(tempStack.top());
                    tempStack.pop();
                }
            }
        }

    private:
        static int getNumStacksFromLine(const std::string& in) {
            // First parse the stack numbers
            std::stringstream sstrStackNums(in);
            std::vector<int> stackNums;
            int num;
            while (sstrStackNums >> num) {
                stackNums.push_back(num);
            }

            auto numStacks = static_cast<int>(stackNums.size());
            return numStacks;
        }

        using StringLines = std::deque<std::string>;

        static Stack getStackFromStringLines(StringLines in, int i) {
            const int col = 1 + 4 * i;
            Stack stack = std::accumulate(in.begin(), in.end(), Stack(), [col](Stack&& acc, const std::string& line){
                if ((col < line.length()) && (line[col] != ' '))
                    acc.push(line[col]);
                return acc;
            });
            return stack;
        }

        static List parseStringData(const std::string& inData) {
            // Go in reverse line order
            StringLines stringDeque;
            auto ss = std::stringstream{inData};
            for (std::string line; std::getline(ss, line, '\n');) {
                if (!line.empty())
                    stringDeque.push_front(line);
            }
            auto numStacks = getNumStacksFromLine(stringDeque.front());
            stringDeque.pop_front();
            List stacks;
            for (int i = 0; i < numStacks; ++i) {
                auto stack = getStackFromStringLines(stringDeque, i);
                stacks.push_back(stack);
            }

            return stacks;
        }

        List _stacks;
    };

    std::string stackToString(CrateStacks::Stack in) {
        std::ostringstream sstr;
        while (!in.empty()) {
            sstr << in.top();
            in.pop();
        }
        std::string str = sstr.str();
        std::reverse(str.begin(), str.end());
        return str;
    }

    class Moves {
    public:
        using MoveList = std::vector<Move>;

        Moves() = default;

        explicit Moves(const std::string& in) : _moves(parseStringData(in)) {

        }

        [[nodiscard]] MoveList getMoveList() const { return _moves; }

    private:
        static MoveList parseStringData(const std::string& inData) {
            auto ss = std::stringstream{inData};
            MoveList ml;

            for (std::string line; std::getline(ss, line, '\n');) {
                Move move(parseMoveLine(line));
                ml.push_back(move);
            }

            return ml;
        }

        static Move parseMoveLine(std::string in) {
            // example: move 17 from 8 to 7
            std::string moveDel = "move";
            std::string fromDel = "from";
            std::string toDel = "to";

            in.erase(0, in.find(moveDel) + moveDel.length());
            const auto fromLoc = in.find(fromDel);
            const auto toLoc = in.find(toDel);
            const auto inLen = in.length();
            const auto numToMoveStr = in.substr(0, fromLoc);
            int numToMove{stoi(numToMoveStr)};
            const auto fromStackStr = in.substr(fromLoc + fromDel.length(), inLen - toLoc - 1);
            int fromStack{stoi(fromStackStr) - 1};
            const auto toStackStr = in.substr(toLoc + toDel.length());
            int toStack{stoi(toStackStr) - 1};

            return Move{numToMove, fromStack, toStack};
        }

        MoveList _moves;
    };

    CrateStacks doMoves(CrateStacks stacks, const Moves& moves, Move::Type type) {
        for (const auto &move: moves.getMoveList()) {
            stacks.doMove(move, type);
        }
        return stacks;
    }

    class CrateMover {
    public:
        CrateMover(const std::string& in, Move::Type type) : _stacks(), _moves() {
            std::tie(_stacks, _moves) = parseStringData(in);
            _stacks = doMoves(_stacks, _moves, type);
        };

        [[nodiscard]] std::string getTopOfStacks() const {
            return _stacks.getTopOfStacks();
        }

    private:
        static std::pair<CrateStacks, Moves> parseStringData(std::string inData) {
            // Split the crate stacks from the move list
            std::string delimiter = "move";
            std::string strCrateStacks = inData.substr(0, inData.find(delimiter));
            inData.erase(0, inData.find(delimiter));
            std::string strMoves = inData;

            CrateStacks cs(strCrateStacks);
            Moves ml(strMoves);

            return {cs, ml};
        }

        CrateStacks _stacks;
        Moves _moves;
    };

}

#endif //ADVENT_OF_CODE_2022_DAY_5_H
