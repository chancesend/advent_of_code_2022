//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_11_H
#define ADVENT_OF_CODE_2022_DAY_11_H

#include <utility>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>
#include <cassert>
#include <queue>

namespace Day11 {

    // TODO: Ugh, global state!
    static int64_t _globalLcm;
    void setGlobalLcm(int64_t globalLcm) {_globalLcm = globalLcm;}

    class Monkey
    {
    public:
        using Item = int64_t;
        using ItemList = std::queue<Item>;
        using ItemCount = int64_t;

        Monkey(const std::string& in, bool doDiv3) : Monkey(parseDataInput(in))
        {
            _doDiv3 = doDiv3;
        }

        Monkey(int num, ItemList startingItems, auto&& op, int divByTest, int throwTrue, int throwFalse);

        int getNumItems() {return static_cast<int>(_items.size());}
        [[nodiscard]] int getDivTest() const {return _divByTest;}

        std::pair<Item, int> inspectItem()
        {
            _inspected++;
            auto item = _items.front();
            _items.pop();
            Item updatedWorryLevel = _operation(item);
            if (_doDiv3)
            {
                updatedWorryLevel /= 3;
            }
            else
            {
                updatedWorryLevel %= _globalLcm;
            }
            int monkeyToThrowTo;
            if (updatedWorryLevel % _divByTest == 0)
            {
                monkeyToThrowTo = _throwMonkeyTrue;
            }
            else
            {
                monkeyToThrowTo = _throwMonkeyFalse;
            }

            return {updatedWorryLevel, monkeyToThrowTo};
        }

        void receiveItem(Item worryLevel)
        {
            _items.push(worryLevel);
        }

        [[nodiscard]] ItemCount getNumItemsInspected() const
        {
            return _inspected;
        }

    private:
        using OperationFcn = std::function<Item(Item)>;

        static Monkey parseDataInput(const std::string& in)
        {
            auto ss = std::stringstream{in};
            std::string line;

            std::getline(ss, line, '\n');
            auto num = parseMonkeyNum(line);

            std::getline(ss, line, '\n');
            auto startingItems = parseStartingItems(line);

            std::getline(ss, line, '\n');
            auto operation = parseOperation(line);

            std::getline(ss, line, '\n');
            auto divByTest = parseDivTest(line);

            std::getline(ss, line, '\n');
            auto monkeyTrue = parseOnTrue(line);

            std::getline(ss, line, '\n');
            auto monkeyFalse = parseOnFalse(line);

            Monkey m(num, startingItems, operation, divByTest, monkeyTrue, monkeyFalse);
            return m;
        }

        static int parseMonkeyNum(const std::string& in)
        {
            std::string monkeyDelimiter = "Monkey";
            auto monkeyNumIndex = in.find("Monkey") + monkeyDelimiter.length() + 1;
            auto monkeyNumStr = in.substr(monkeyNumIndex, 1);
            auto monkeyNum = std::stoi(monkeyNumStr);
            return monkeyNum;
        }

        static ItemList parseStartingItems(std::string in)
        {
            ItemList items;
            std::string del = "Starting items:";
            auto index = in.find(del) + del.length() + 1;
            auto subIn = in.erase(0, index);

            auto ss = std::stringstream{subIn};
            while( ss.good() )
            {
                std::string itemStr;
                getline( ss, itemStr, ',' );
                items.push( std::stoi(itemStr) );
            }
            return items;
        }

        static OperationFcn parseOperation(const std::string& in)
        {
            std::string delimiter {"new = old"};
            auto opIdx = in.find(delimiter) + delimiter.length() + 1;
            auto opStr = in.substr(opIdx, 1);
            auto param2Str = in.substr(opIdx + 2);
            int param2 {0};
            bool param2IsOld;
            if (std::isdigit(param2Str[0]))
            {
                param2 = std::stoi(param2Str);
                param2IsOld = false;
            }
            else
            {
                param2IsOld = true;
            }

            auto opFcn = [opStr, param2IsOld, param2](Item in){
                if (opStr == "*")
                {
                    return (param2IsOld) ? in * in : in * param2;
                }
                else if (opStr == "+")
                {
                    return (param2IsOld) ? in + in : in + param2;
                }
                return (Item)0;
            };

            return opFcn;
        }

        static int parseDivTest(const std::string& in)
        {
            std::string del = "divisible by";
            auto index = in.find(del) + del.length() + 1;
            auto numStr = in.substr(index);
            auto num = std::stoi(numStr);
            return num;
        }

        static int parseOnTrue(const std::string& in)
        {
            std::string del = "throw to monkey";
            auto index = in.find(del) + del.length() + 1;
            auto numStr = in.substr(index);
            auto num = std::stoi(numStr);
            return num;
        }

        static int parseOnFalse(const std::string& in)
        {
            std::string del = "throw to monkey";
            auto index = in.find(del) + del.length() + 1;
            auto numStr = in.substr(index);
            auto num = std::stoi(numStr);
            return num;
        }

        ItemList _items;
        OperationFcn _operation;
        int _divByTest;
        int _throwMonkeyTrue;
        int _throwMonkeyFalse;
        ItemCount _inspected{0};

        bool _doDiv3;
    };

    Monkey::Monkey(int num, Monkey::ItemList startingItems, auto &&op, int divByTest, int throwTrue, int throwFalse) :
            _items(std::move(std::move(startingItems))),
            _operation(op),
            _divByTest(divByTest),
            _throwMonkeyTrue(throwTrue),
            _throwMonkeyFalse(throwFalse),
            _doDiv3(true)
    {
    }

    class MonkeyBusiness
    {
    public:
        using MonkeyList = std::vector<Monkey>;
        using ItemCountList = std::vector<Monkey::ItemCount>;

        explicit MonkeyBusiness(const std::string& in, bool doDiv3 = true) : _monkeys(parseDataInput(in, doDiv3))
        {
        }

        ItemCountList getNumItemsSeenAfterRounds(int num)
        {
            for (int i = 0; i < num; ++i)
            {
                doRound();
            }

            ItemCountList itemsSeenPerMonkey;
            for (auto& monkey: _monkeys)
            {
                itemsSeenPerMonkey.push_back(monkey.getNumItemsInspected());
            }
            return itemsSeenPerMonkey;
        }

        void doRound()
        {
            for (auto& monkey: _monkeys)
            {
                auto numItems = monkey.getNumItems();
                for (int i = 0; i < numItems; ++i)
                {
                    Monkey::Item wl;
                    int nextMonkey;
                    std::tie(wl, nextMonkey) = monkey.inspectItem();
                    _monkeys[nextMonkey].receiveItem(wl);
                }
            }
        }

    private:
        static MonkeyList parseDataInput(const std::string& in, bool doDiv3)
        {
            MonkeyList ml;
            auto ss = std::stringstream{in};

            std::string line;
            size_t monkeyBegin = 0;

            const std::string monkeyDelimiter = "Monkey";
            size_t monkeyEnd;

            int globalLcm = 1;
            while(std::getline(ss, line, '\n'))
            {
                monkeyEnd = in.find(monkeyDelimiter, monkeyBegin + monkeyDelimiter.length());
                auto monkeyStr = in.substr(monkeyBegin, in.find(monkeyDelimiter, monkeyBegin + monkeyDelimiter.length()));
                // Split the monkeys apart
                Monkey monkey(monkeyStr, doDiv3);
                globalLcm *= monkey.getDivTest();
                ml.push_back(monkey);
                monkeyBegin = monkeyEnd;
                if (monkeyEnd == std::string::npos)
                    break;
            }

            setGlobalLcm(globalLcm);

            return ml;
        }

    public:
        MonkeyList  _monkeys;
    };

    Monkey::ItemCount getMonkeyBusiness(MonkeyBusiness::ItemCountList& itemsSeen)
    {
        std::sort(std::begin(itemsSeen), std::end(itemsSeen));
        auto mb = itemsSeen.rbegin()[0] * itemsSeen.rbegin()[1];
        return mb;
    }

}

#endif //ADVENT_OF_CODE_2022_DAY_11_H
