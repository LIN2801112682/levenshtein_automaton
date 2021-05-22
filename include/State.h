#ifndef LEVENSHTEIN_AUTOMATON_STATE_H
#define LEVENSHTEIN_AUTOMATON_STATE_H

#include <vector>
#include <unordered_set>
#include "./Position.h"
#include "./AugBitSet.h"
namespace la{
    class State{
    private:
        const std::vector<Position*> memberPositionArray;
    public:
        State(std::vector<State *> &stateCollection, int &maxEditDistance);
        State(std::unordered_set<State *> &stateCollection, int &maxEditDistance);
        State(std::vector<Position*> &memberPositionArray) : memberPositionArray(memberPositionArray){};
        State(State *s, Position *p);
        int getMinimalBoundary();
        State *transition(int maxEditDistance, AugBitSet *relevantSubwordCharacteristicVector);
        bool canBeState(State *state, Position *prospectivePosition, int maxEditDistance);
        int getMinimumBoundariesDifference(State *state1, State *state2);

        std::vector<Position *> getMemberPositions();

        AugBitSet *
        getRelevantSubwordCharacteristicVector(int maxEditDistance, const std::wstring &automatonString,
                                               wchar_t letter);

        std::string toString();

        State *transition(int maxEditDistance, const std::wstring &automatonString, wchar_t letter);
    };
}
#endif
