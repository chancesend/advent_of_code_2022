//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_13_H
#define ADVENTOFCODE2022_DAY_13_H

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

namespace Day13 {

    class List;

    using ListPtr = std::shared_ptr<List>;
    using Elem = std::variant<int, ListPtr>;

    class List
    {
    public:
        List(std::string& in)
        {
            _str = in;
            _vec = parseDataInput(in);
        }

        List(int val) {
            _vec.push_back(val);
        }

        std::vector<Elem> getVec() const { return _vec; }
        std::string print() const
        {
            std::stringstream ss;
            ss << "[";
            for (auto iter = _vec.begin(); iter != _vec.end(); ++iter)
            {
                if (std::distance(_vec.begin(), iter) > 0)
                    ss << ",";
                if (std::holds_alternative<int>(*iter))
                {
                    auto num = std::get<int>(*iter);
                    ss << num;
                }
                else if (std::holds_alternative<ListPtr>(*iter))
                {
                    auto list = std::get<ListPtr>(*iter);
                    ss << list->print();
                }
            }
            ss << "]";
            return ss.str();
        }

    private:
        std::vector<Elem> parseDataInput(std::string& in)
        {
            std::vector<Elem> vec;
            // Remove the first bracket
            in = in.substr(1);
            // Parse between the commas
            std::stringstream ss(in);
            while (!in.empty()) {
                auto nextIndex = in.find_first_not_of("0123456789");
                if (in[0] == '[')
                {
                    // This is a sub-list. Parse it like a list, then advance read pointer past sublist
                    auto list = std::make_shared<List>(in);
                    in = in.substr(1);
                    vec.push_back(static_cast<ListPtr>(list));
                    assert(std::holds_alternative<ListPtr>(vec.back()));
                }
                else if (in[0] == ']')
                {
                    // Close out this sublist and return
                    return vec;
                }
                else if (in[0] == ',')
                    in = in.substr(1);
                else {
                    // Else we have a new valu
                    auto elemStr = in.substr(0, nextIndex);
                    in = in.substr(nextIndex);
                    auto elem = static_cast<int>(stoi(elemStr));
                    vec.push_back(elem);
                    assert(std::holds_alternative<int>(vec.back()));
                }

            }
            return vec;
        }

        std::vector<Elem> _vec;
        std::string _str;
    };

    std::ostream &operator<<(std::ostream &out, List &list)
    {
        out << list.print();
        return out;
    }

    enum OrderDecision
    {
        RIGHT,
        WRONG,
        NO_DECISION
    };

    OrderDecision pairInRightOrder(const List& left, const List& right)
    {
        int li = 0;
        int ri = 0;
        const auto& leftVec = left.getVec();
        const auto& rightVec = right.getVec();
        while((li < leftVec.size()) && (ri < rightVec.size()))
        {
            bool leftIsList = std::holds_alternative<ListPtr>(leftVec[li]);
            bool rightIsList = std::holds_alternative<ListPtr>(rightVec[ri]);
            if (!leftIsList && !rightIsList)
            {
                auto l = std::get<int>(leftVec[li]);
                auto r = std::get<int>(rightVec[ri]);
                if(l < r)
                    return RIGHT;
                else if (l > r)
                    return WRONG;
            }
            else if (leftIsList && rightIsList)
            {
                auto l = std::get<ListPtr>(leftVec[li]);
                auto r = std::get<ListPtr>(rightVec[ri]);
                auto order = pairInRightOrder(*l, *r);
                if (order != NO_DECISION)
                    return order;
            }
            else
            {
                if (!leftIsList)
                {
                    List leftList(std::get<int>(leftVec[li]));
                    auto r = std::get<ListPtr>(rightVec[ri]);
                    auto order = pairInRightOrder(leftList, *r);
                    if (order != NO_DECISION)
                        return order;
                }
                if (!rightIsList)
                {
                    auto l = std::get<ListPtr>(leftVec[li]);
                    List rightList(std::get<int>(rightVec[ri]));
                    auto order = pairInRightOrder(*l, rightList);
                    if (order != NO_DECISION)
                        return order;
                }
            }
            li++;
            ri++;
        }
        if (ri < rightVec.size())
            return RIGHT;
        if (li < leftVec.size())
            return WRONG;
        return NO_DECISION;
    }

bool operator< (const List& left, const List& right)
{
    auto order = pairInRightOrder(left, right);
    return order == RIGHT;
}

class PairFinder
{
public:
    using ListPair = std::pair<List, List>;

    PairFinder(const std::string& in) : _lps(parseDataInput(in)) {}

    std::vector<int> indicesOfPairsInRightOrder()
    {
        std::vector<int> indicesOfPairs;
        int indexCnt = 1;
        for (const auto& lists: _lps)
        {
            const auto& list1 = lists.first;
            const auto& list2 = lists.second;
            if (list1 < list2)
                indicesOfPairs.push_back(indexCnt);
            indexCnt++;
        }
        return indicesOfPairs;
    }

private:
    static std::vector<ListPair> parseDataInput(const std::string& in)
    {
        auto ss = std::stringstream{in};
        std::string line;

        std::vector<ListPair> lp;

        // We know that the format is list1, list2, line-break
        while(std::getline(ss, line, '\n'))
        {
            List list1(line);
            std::getline(ss, line, '\n');
            List list2(line);
            std::getline(ss, line, '\n');
            lp.push_back({list1, list2});
        }

        return lp;
    }

    std::vector<ListPair> _lps;
};

    class PacketDecoder
    {
    public:
        PacketDecoder(const std::string& in) : _vl(parseDataInput(in))
        {
            // Insert the divider packets
            std::string div1 = "[[6]]";
            std::string div2 = "[[2]]";
            _vl.push_back(List(div1));
            _vl.push_back(List(div2));
        }

        std::vector<int> getDividerPackets()
        {
            std::sort(_vl.begin(), _vl.end());
            std::string div1{"[[2]]"};
            auto div1Iter = std::find_if(_vl.begin(), _vl.end(), [div=div1](const List& list){
                return list.print() == div;
            });
            std::string div2{"[[6]]"};
            auto div2Iter = std::find_if(_vl.begin(), _vl.end(), [div=div2](const List& list){
                return list.print() == div;
            });

            std::vector<int> vec;
            auto div1Idx = std::distance(_vl.begin(), div1Iter);
            vec.push_back(div1Idx+1);
            auto div2Idx = std::distance(_vl.begin(), div2Iter);
            vec.push_back(div2Idx+1);

            return vec;
        }

    private:
        static std::vector<List> parseDataInput(const std::string& in)
        {
            auto ss = std::stringstream{in};
            std::string line;

            std::vector<List> vl;

            // We know that the format is list1, list2, line-break
            while(std::getline(ss, line, '\n'))
            {
                if (line == "")
                    continue;
                List list(line);
                vl.push_back(list);
            }

            return vl;
        }

        std::vector<List> _vl;
    };

}

#endif //ADVENTOFCODE2022_DAY_13_H
