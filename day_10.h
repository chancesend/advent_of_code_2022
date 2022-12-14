//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_10_H
#define ADVENTOFCODE2022_DAY_10_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>
#include <cassert>

namespace Day10 {

    class Command {
    public:
        Command(const std::string& in) {
            auto cmdString = in.substr(0, kADDX.length());
            if (in.length() > kADDX.length()) {
                auto paramString = in.substr(kADDX.length() + 1);
                _param = std::stoi(paramString);
            }
            if (cmdString == kADDX) {
                _cmd = Cmd::ADDX;
                _cycles = 2;
            } else if (cmdString == kNOOP) {
                _cmd = Cmd::NOOP;
                _cycles = 1;
            }
        }

        std::vector<int> getAddEquiv() {
            if (_cmd == ADDX) {
                return {0, _param};
            }
            if (_cmd == NOOP)
                return {0};

            return {};
        }

    private:
        const std::string kADDX{"addx"};
        const std::string kNOOP{"noop"};
        enum Cmd {
            ADDX = 2,
            NOOP = 1
        };

        Cmd _cmd;
        int _param;
        int _cycles;
    };

    class StrengthChecker {
    public:
        StrengthChecker(std::string in) : _program(parseStringData(in)) {}

        std::vector<int> getAddList() {
            std::vector<int> addList;
            for (auto line: _program) {
                auto addEquiv = line.getAddEquiv();
                addList.insert(addList.end(), addEquiv.begin(), addEquiv.end());
            }
            return addList;
        }

        std::vector<int> getXAccumList(std::vector<int> addList) {
            std::vector<int> xAddrList(addList.size());
            addList[0] += _xRegStart;
            std::partial_sum(addList.begin(), addList.end(), xAddrList.begin());
            return xAddrList;
        }

        std::vector<int> getSignalStrengths(const std::vector<int>& cycles) {
            auto xAccumList = getXAccumList(getAddList());

            std::vector<int> strengthList(cycles.size());
            std::transform(cycles.begin(), cycles.end(), strengthList.begin(), [&xAccumList = xAccumList](int cycle) {
                return xAccumList[cycle - 1] * cycle;
            });
            return strengthList;
        }


    private:
        using Program = std::vector<Command>;

        static Program parseStringData(std::string in) {
            Program program;
            auto ss = std::stringstream{in};

            for (std::string line; std::getline(ss, line, '\n');) {
                Command cmd(line);
                program.push_back(cmd);
            }

            return program;
        }

        Program _program;
        const int _xRegStart{1};
    };

    std::string getCrtOutput(const std::vector<int>& xAccumList) {
        auto isLit = [](int spritePos, int crtPos) {
            return std::abs(spritePos - crtPos) <= 1;
        };

        auto getChar = [](bool isLit) {
            return isLit ? '#' : '.';
        };

        std::string out;
        const int kScanLine = 40;
        int i {0};
        int xAccum = xAccumList[0];
        do {
            if ((i > 0) && (i % kScanLine == 0)) {
                out += '\n';
            }
            out += getChar(isLit(xAccum, i % kScanLine));
            xAccum = xAccumList[i];
            ++i;
        } while (i < xAccumList.size());

        return out;
    }

}

#endif //ADVENTOFCODE2022_DAY_10_H
