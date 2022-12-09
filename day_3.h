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

    Item itemInBothCompartments() const
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

    Item::List lower() const {return _firstHalf;}
    Item::List upper() const {return _secondHalf;}

private:
    static std::pair<Item::List, Item::List> parseStringData(std::string inData)
    {
        const int splitPoint = inData.length() / 2;

        auto listBuilder = [](Item::List&& acc, char item) -> Item::List{
            acc.push_back(Item(item));
            return std::move(acc);
        };

        auto first = std::accumulate(inData.begin(), inData.begin() + splitPoint, Item::List(), listBuilder);
        auto second = std::accumulate(inData.begin() + splitPoint, inData.end(), Item::List(), listBuilder);

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
        for (auto item: inData) {
            list.push_back(Item(item));
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
        auto intersectionBuilder = [](Item::List&& intersection, SortedRucksack rs){
            Item::List newIntersection;
            std::set_intersection(intersection.begin(), intersection.end(),
                                  rs.getItems().begin(), rs.getItems().end(),
                                  std::back_inserter(newIntersection));
            return std::move(newIntersection);

        };
        auto badge = std::accumulate(_rucksacks.begin(), _rucksacks.end(), Item::List(_rucksacks[0].getItems()), intersectionBuilder);
        return badge[0];
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
                groups.push_back(std::move(eg));
                tempRsl.clear();
            }
        }
        return groups;
    }

    Item::List commonItems()
    {
        Item::List itemList = std::accumulate(_rsl.begin(), _rsl.end(), Item::List(), [](Item::List&& acc, const Rucksack& rs){
            acc.push_back(rs.itemInBothCompartments());
            return acc;
        });
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
