#include "levenshtein_automaton/State.h"

namespace la
{
    State::State(const std::vector<Position> &memberPositionArray)
        : memberPositionArray{memberPositionArray} {}

    State::State(const State &s, const Position &p)
    {
        memberPositionArray = s.memberPositionArray;
        memberPositionArray.emplace_back(p);
        //todo
        //std::sort(std::begin(memberPositionArray), std::end(memberPositionArray));
    }

    State::State(const std::vector<State> &stateCollection, int maxEditDistance)
    {
        std::unordered_set<Position> prospectiveMemberPositionCollection;
        for (const auto &state : stateCollection)
        {
            for (const auto &position : state.memberPositionArray)
            {
                // todo
                //prospectiveMemberPositionCollection.emplace(position);
                prospectiveMemberPositionCollection.emplace(position);
            }
        }

        auto it1 = std::begin(prospectiveMemberPositionCollection);
        while (it1 != std::end(prospectiveMemberPositionCollection))
        {
            // confuse
            Position position1{*it1++};
            auto it2 = std::begin(prospectiveMemberPositionCollection);
            while (it2 != std::end(prospectiveMemberPositionCollection))
            {
                // confuse
                Position position2{*it2++};
                // todo
                /*
                if (position2.subsumes(position1, maxEditDistance))
                {
                    // confuse
                    prospectiveMemberPositionCollection.erase(it1);
                    break;
                }
                */
            }
        }
        for (auto &&position : prospectiveMemberPositionCollection)
        {
            memberPositionArray.emplace_back(std::move(position));
        }
        // todo
        //std::sort(std::begin(memberPositionArray), std::end(memberPositionArray));
    }

    int
    State::getMinimalBoundary()
    {
        // todo
        //return memberPositionArray[0].getI();
        return 0;
    }

    std::vector<Position>
    State::getMemberPositions()
    {
        return memberPositionArray;
    }

    AugBitSet
    State::getRelevantSubwordCharacteristicVector(int maxEditDistance, const std::string &automatonString, char letter)
    {
        // todo
        int minimalPositionBoundary{0};
        //int minimalPositionBoundary{memberPositionArray[0].getI()};
        int relevantSubwordSize{std::min(2 * maxEditDistance + 1, static_cast<int>(automatonString.length()) - minimalPositionBoundary)};
        std::string relevantSubword{automatonString.substr(minimalPositionBoundary, relevantSubwordSize)};
        // todo
        AugBitSet relevantSubwordCharacteristicVector{};
        // AugBitSet relevantSubwordCharacteristicVector{relevantSubwordSize};
        for (int i = 0; i < relevantSubwordSize; ++i)
        {
            if (relevantSubword.at(i) == letter)
            {
                // todo
                //relevantSubwordCharacteristicVector.set(i);
            }
        }
        return relevantSubwordCharacteristicVector;
    }

    State
    State::transition(int maxEditDistance, const AugBitSet &relevantSubwordCharacteristicVector)
    {
        //todo
        std::vector<State> newStateHashSet;
        //std::unordered_set<State> newStateHashSet;
        // todo
        int boundaryOffset{0};
        //int boundaryOffset{memberPositionArray[0].getI()};
        for (const auto &p : memberPositionArray)
        {
            // todo
            /*
            State transitionState = p.transition(maxEditDistance, p.getI() - boundaryOffset, relevantSubwordCharacteristicVector);
            if (transitionState != nullptr)
            {
                newStateHashSet.insert(transitionState);
            }
            */
        }
        // todo
        //return newStateHashSet.empty() ? nullptr : State{newStateHashSet, maxEditDistance};
        return State{newStateHashSet, maxEditDistance};
    }

    State
    State::transition(int maxEditDistance, const std::string &automatonString, char letter)
    {
        return transition(maxEditDistance, getRelevantSubwordCharacteristicVector(maxEditDistance, automatonString, letter));
    }

    bool State::canBeState(const State &state, const Position &prospectivePosition, int maxEditDistance)
    {
        for (auto currentMemberPosition : state.memberPositionArray)
        {
            // todo
            /*
            if (currentMemberPosition->subsumes(prospectivePosition, maxEditDistance))
            {
                return false;
            }
            */
        }
        return true;
    }

    int State::getMinimumBoundariesDifference(const State &state1, const State &state2)
    {
        // todo
        return 0;
        //return std::abs(state1.memberPositionArray[0].getI() - state2.memberPositionArray[0].getI());
    }
}