//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_2_H
#define ADVENT_OF_CODE_2022_DAY_2_H

#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include <map>
#include <numeric>

class Round
{
public:
    enum Shape {
        Rock = 1,
        Paper = 2,
        Scissors = 3
    };

    enum Parser {
        OldParse = 0,
        NewParse = 1
    };

    Round(Shape opp, Shape yours) : _opp(opp), _yours(yours) {}
    Round(const std::string& round, Parser parser)
    {
        _opp = charToMove(round.c_str()[0]);

        if (parser == OldParse) {
            _yours = charToMove(round.c_str()[2]);
        }
        else if (parser == NewParse)
        {
            auto outcome = charToOutcome(round.c_str()[2]);
            _yours = getMoveForOutcome(_opp, outcome);
        }
    }

    [[nodiscard]] int score() const
    {
        return shapeScore() + outcomeScore();
    }

private:
    enum Outcome
    {
        Lose = 0,
        Draw = 3,
        Win = 6
    };

    [[nodiscard]] Shape getMoveForOutcome(Shape opp, Outcome outcome) const
    {
        const auto round = std::find_if(outcomeList.cbegin(), outcomeList.cend(),
                                        [opp, outcome](RoundState state){
                                            return state.opp == opp && state.outcome == outcome;
                                        });
        return round->yours;
    }

    [[nodiscard]] int shapeScore() const
    {
        return static_cast<int>(_yours);
    }

    // Order is Opponent, Yours, Outcome
    struct RoundState
    {
        Shape opp;
        Shape yours;
        Outcome outcome;
    };
    const std::vector<RoundState> outcomeList {
            {Shape::Rock, Shape::Rock, Outcome::Draw},
            {Shape::Rock, Shape::Paper, Outcome::Win},
            {Shape::Rock, Shape::Scissors, Outcome::Lose},

            {Shape::Paper, Shape::Rock, Outcome::Lose},
            {Shape::Paper, Shape::Paper, Outcome::Draw},
            {Shape::Paper, Shape::Scissors, Outcome::Win},

            {Shape::Scissors, Shape::Rock, Outcome::Win},
            {Shape::Scissors, Shape::Paper, Outcome::Lose},
            {Shape::Scissors, Shape::Scissors, Outcome::Draw},
    };

    [[nodiscard]] int outcomeScore() const
    {
        const auto round = std::find_if(outcomeList.cbegin(), outcomeList.cend(),
                                          [opp=_opp, yours=_yours](RoundState state){
            return state.opp == opp && state.yours == yours;
        });
        return static_cast<int>(round->outcome);
    }

    static Shape charToMove(char in) {
        const std::map<char, Shape> kMoveMap =
            {
                {'A', Shape::Rock},
                {'B', Shape::Paper},
                {'C', Shape::Scissors},
                {'X', Shape::Rock},
                {'Y', Shape::Paper},
                {'Z', Shape::Scissors},
            };
        return kMoveMap.at(in);
    }

    static Outcome charToOutcome(char in) {
        const std::map<char, Outcome> kOutcomeMap =
                {
                        {'X', Outcome::Lose},
                        {'Y', Outcome::Draw},
                        {'Z', Outcome::Win},
                };
        return kOutcomeMap.at(in);
    }

    Shape _opp;
    Shape _yours;
};

class StrategyGuide {
public:
    using RoundList = std::vector<Round>;

    StrategyGuide(const std::string& in, Round::Parser parser)
    {
        _roundList = parseStringData(in, parser);
    }

    [[nodiscard]] int getTotalScore() const
    {
        auto totalScore = std::accumulate(_roundList.begin(), _roundList.end(), 0, [](int acc, const Round& round){
            return acc + round.score();
        });
        return totalScore;
    }

private:
    static RoundList parseStringData(const std::string& inData, Round::Parser parser)
    {
        RoundList result;
        auto ss = std::stringstream{inData};

        for (std::string line; std::getline(ss, line, '\n');) {
            Round thisRound(line, parser);
            result.push_back(thisRound);
        }

        return result;
    }

    RoundList _roundList;
};

#endif //ADVENT_OF_CODE_2022_DAY_2_H
