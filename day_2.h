//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_2_H
#define ADVENTOFCODE2022_DAY_2_H

#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include <map>

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
    Round(std::string round, Parser parser)
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

    int score() const
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

    Shape getMoveForOutcome(Shape opp, Outcome outcome) const
    {
        const auto round = std::find_if(outcomeList.cbegin(), outcomeList.cend(),
                                        [opp, outcome](RoundState state){
                                            return state.opp == opp && state.outcome == outcome;
                                        });
        return round->yours;
    }

    int shapeScore() const
    {
        return static_cast<int>(_yours);
    }

    // Order is Opponent, Yours, Outcome
    using OutcomeEntry = std::tuple<Shape, Shape, Outcome>;
    using Moves = std::pair<Shape, Shape>;
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

    int outcomeScore() const
    {
        const auto round = std::find_if(outcomeList.cbegin(), outcomeList.cend(),
                                          [opp=_opp, yours=_yours](RoundState state){
            return state.opp == opp && state.yours == yours;
        });
        return static_cast<int>(round->outcome);
    }

    static Shape charToMove(char in) {
        Shape shape;
        if (in == 'A')
            shape = Shape::Rock;
        else if (in == 'B')
            shape = Shape::Paper;
        else if (in == 'C')
            shape = Shape::Scissors;
        else if (in == 'X')
            shape = Shape::Rock;
        else if (in == 'Y')
            shape = Shape::Paper;
        else if (in == 'Z')
            shape = Shape::Scissors;
        return shape;
    }

    static Outcome charToOutcome(char in) {
        Outcome outcome;
        if (in == 'X')
            outcome = Outcome::Lose;
        else if (in == 'Y')
            outcome = Outcome::Draw;
        else if (in == 'Z')
            outcome = Outcome::Win;
        return outcome;
    }

    Shape _opp;
    Shape _yours;
};

class StrategyGuide {
public:
    using RoundList = std::vector<Round>;

    StrategyGuide(std::string in, Round::Parser parser)
    {
        _roundList = parseStringData(in, parser);
    }

    int getTotalScore() const
    {
        int totalScore = 0;
        for (const auto& round: _roundList)
        {
            totalScore += round.score();
        }
        return totalScore;
    }

private:
    RoundList parseStringData(std::string inData, Round::Parser parser)
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

#endif //ADVENTOFCODE2022_DAY_2_H
