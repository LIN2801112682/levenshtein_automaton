#pragma once

#include <vector>
#include <unordered_set>
#include <iostream>
#include <>

namespace la
{
    // todo
    class Position
    {
    };
    class AugBitSet
    {
    };

    class State
    {
    private:
        std::vector<Position> memberPositionArray;

    public:
        State(const std::vector<Position> &memberPositionArray);
        State(const State &s, const Position &p);
        State(const std::vector<State> &stateCollection, int maxEditDistance);
        int getMinimalBoundary();
        std::vector<Position> getMemberPositions();
        AugBitSet getRelevantSubwordCharacteristicVector(
            int maxEditDistance, const std::string &automatonString, char letter);
        State transition(int maxEditDistance, const AugBitSet &relevantSubwordCharacteristicVector);
        State transition(int maxEditDistance, const std::string &automatonString, char letter);
        std::string toString();
        bool canBeState(const State &state, const Position &prospectivePosition, int maxEditDistance);
        int getMinimumBoundariesDifference(const State &state1, const State &state2);
        // todo
        //friend std::ostream &operator<<(std::ostream &os, State &state);
    };
};