//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_4_H
#define ADVENTOFCODE2022_DAY_4_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>

class ElfAssignment
{
public:
    using SectionId = int;
    using SectionList = std::vector<SectionId>;

    ElfAssignment() = default;
    ElfAssignment(std::string in)
    {
        std::string delimiter = "-";
        auto strId1 = in.substr(0, in.find(delimiter));
        SectionId id1(stoi(strId1));
        in.erase(0, in.find(delimiter) + delimiter.length());
        auto strId2 = in;
        SectionId id2(stoi(strId2));
        _sections.reserve(id2-id1+1);
        for (SectionId i = id1; i <= id2; ++i)
        {
            _sections.push_back(i);
        }
    }

    SectionList getSections() const {return _sections;}

private:
    SectionList _sections;
};

class SectionAssignment
{
public:
    using List = std::vector<SectionAssignment>;
    using ElfAssignmentList = std::vector<ElfAssignment>;

    SectionAssignment(const std::string& in) : _elves(parseStringData(in)) {}

    bool isOverlapping() const
    {
        ElfAssignment::SectionList intersection;
        const auto sections1 = _elves[0].getSections();
        const auto sections2 = _elves[1].getSections();
        std::set_intersection(sections1.begin(), sections1.end(),
                              sections2.begin(), sections2.end(),
                              std::back_inserter(intersection));
        auto isOverlapping = (intersection.size() > 0);
        return isOverlapping;
    }

    bool isFullyContained() const
    {
        ElfAssignment::SectionList intersection;
        auto sections1 = _elves[0].getSections();
        auto sections2 = _elves[1].getSections();
        std::set_intersection(sections1.begin(), sections1.end(),
                              sections2.begin(), sections2.end(),
                              std::back_inserter(intersection));
        auto isFullyContained = (intersection.size() == std::min(sections1.size(), sections2.size()));
        return isFullyContained;
    }

private:
    static ElfAssignmentList parseStringData(std::string in) {
        ElfAssignmentList eal;
        std::string delimiter = ",";
        while(in.size())
        {
            ElfAssignment ea(in.substr(0, in.find(delimiter)));
            eal.push_back(ea);
            auto delPos = in.find(delimiter);
            if (delPos == std::string::npos)
                break;
            in.erase(0, delPos + delimiter.length());
        }
        return eal;
    }

    ElfAssignmentList _elves;
};

class SectionAssignments
{
public:
    SectionAssignments(const std::string& in) : _assignments(parseStringData(in))
    {

    }

    int numPairsOverlapping() const
    {
        int num = std::accumulate(_assignments.begin(), _assignments.end(), 0,
                                  [](int acc, SectionAssignment ass){
                                      acc += ass.isOverlapping() ? 1 : 0;
                                      return acc;
                                  });
        return num;
    }

    int numPairsFullyContained() const
    {
        int num = std::accumulate(_assignments.begin(), _assignments.end(), 0,
                              [](int acc, SectionAssignment ass){
            acc += ass.isFullyContained() ? 1 : 0;
            return acc;
        });
        return num;
    }

private:
    static SectionAssignment::List parseStringData(const std::string& inData)
    {
        SectionAssignment::List list;
        auto ss = std::stringstream{inData};

        for (std::string line; std::getline(ss, line, '\n');) {
            SectionAssignment sa(line);
            list.push_back(sa);
        }

        return list;
    }

    SectionAssignment::List _assignments;
};

#endif //ADVENTOFCODE2022_DAY_3_H
