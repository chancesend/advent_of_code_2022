//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_3_H
#define ADVENTOFCODE2022_DAY_3_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>

class Item {
public:
    using List = std::vector<Item>;

    Item(char in) : _item(in) {}

    int priority() const {
        int priority;
        char shift;
        if ((_item >= 'a') && (_item <= 'z'))
            shift = 'a' - 1;
        else if ((_item >= 'A') && (_item <= 'Z'))
            shift = 'A' - 27;
        priority = static_cast<int>(_item - shift);
        return priority;
    }

    char get() {return _item;}

    bool operator< (Item rhl)
    {
        return priority() < rhl.priority();
    }

private:
    char _item;
};

class Rucksack {
public:
    using List = std::vector<Rucksack>;

    Rucksack(std::string in) {
        std::tie(_firstHalf, _secondHalf) = parseStringData(in);
    }

    Item itemInBothCompartments()
    {
        Item::List intersection;
        Item::List sortedFirst = lower();
        Item::List sortedSecond = upper();
        std::sort(sortedFirst.begin(), sortedFirst.end());
        std::sort(sortedSecond.begin(), sortedSecond.end());
        std::set_intersection(sortedFirst.begin(), sortedFirst.end(),
                              sortedSecond.begin(), sortedSecond.end(),
                              std::back_inserter(intersection));
        return intersection[0];
    }

    Item::List lower() {return _firstHalf;}
    Item::List upper() {return _secondHalf;}

private:
    static std::pair<Item::List, Item::List> parseStringData(std::string inData)
    {
        const int compartmentSize = inData.length() / 2;
        Item::List first;
        Item::List second;

        for (int i = 0; i < compartmentSize; ++i) {
            first.push_back(Item(inData[i]));
            second.push_back(Item(inData[compartmentSize + i]));
        }

        return {first, second};
    }

    Item::List _firstHalf;
    Item::List _secondHalf;
};

class SortedRucksack : public Rucksack {
public:
    using List = std::vector<SortedRucksack>;

    SortedRucksack(Rucksack rs) : Rucksack(rs) {
        Item::List rsUnion;
        Item::List sortedFirst = lower();
        Item::List sortedSecond = upper();
        std::sort(sortedFirst.begin(), sortedFirst.end());
        std::sort(sortedSecond.begin(), sortedSecond.end());
        std::set_union(sortedFirst.begin(), sortedFirst.end(),
                              sortedSecond.begin(), sortedSecond.end(),
                              std::back_inserter(_items));
        std::sort(_items.begin(), _items.end());
    }

    Item::List getItems() const
    {
        return _items;
    }

private:
    static Item::List parseStringData(std::string inData)
    {
        Item::List list;
        for (int i = 0; i < inData.size(); ++i) {
            list.push_back(Item(inData[i]));
        }

        return list;
    }

    Item::List _items;
};

int sumOfPriorities(Item::List items)
{
    auto sum = std::accumulate(items.begin(), items.end(), 0, [](int acc, const Item& item) {
        return acc + item.priority();
    });
    return sum;
}

class ElfGroup
{
public:
    ElfGroup(Rucksack::List rucksacks) {
        for (auto& rs: rucksacks)
        {
            _rucksacks.push_back(SortedRucksack(rs));
        }
    }

    Item getBadge() const {
        Item::List intersection{_rucksacks[0].getItems()};
        for (auto rs: _rucksacks)
        {
            Item::List newIntersection;
            auto items = rs.getItems();
            std::set_intersection(intersection.begin(), intersection.end(),
                                  items.begin(), items.end(),
                                  std::back_inserter(newIntersection));
            intersection = newIntersection;
        }
        return intersection[0];
    }

private:
    SortedRucksack::List _rucksacks;
};

class Rucksacks
{
public:
    using ElfGroups = std::vector<ElfGroup>;
    Rucksacks(std::string in) {
        _rsl = parseStringData(in);
    }

    ElfGroups getElfGroups()
    {
        const int groupSize = 3;
        ElfGroups groups;
        Rucksack::List tempRsl;
        for (int i = 0; i < _rsl.size(); ++i)
        {
            tempRsl.push_back(_rsl[i]);
            if (i % groupSize == groupSize - 1)
            {
                ElfGroup eg(tempRsl);
                groups.push_back(eg);
                tempRsl.clear();
            }
        }
        return groups;
    }

    Item::List commonItems()
    {
        Item::List itemList;
        for(auto& rucksack: _rsl)
        {
            itemList.push_back(rucksack.itemInBothCompartments());
        }
        return itemList;
    }

private:
    Rucksack::List parseStringData(std::string inData)
    {
        Rucksack::List result;
        auto ss = std::stringstream{inData};

        for (std::string line; std::getline(ss, line, '\n');) {
            Rucksack rucksack(line);
            result.push_back(rucksack);
        }

        return result;
    }

    Rucksack::List _rsl;
};

int sumOfGroupBadges(Rucksacks::ElfGroups egs)
{
    auto sum = std::accumulate(egs.begin(), egs.end(), 0, [](int acc, const ElfGroup& eg) {
        return acc + eg.getBadge().priority();
    });
    return sum;
}

#endif //ADVENTOFCODE2022_DAY_3_H
