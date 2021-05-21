#include <cstring>
#include "../include/levenshtein_automaton/State.h"
#include "../include/levenshtein_automaton/Position.h"
#include "../include/levenshtein_automaton/ParametricState.h"
#include <string>
#include <algorithm>
#include <stack>
#include"typeinfo"

namespace std {
    template<>
    struct std::hash<std::vector<int>> : public __hash_base<size_t, std::vector<int>> {
        size_t operator()(const std::vector<int> &__val) const noexcept {
            size_t s{0};
            for (auto &e : __val)
                s += std::hash<int>()(e);
            return s;
        }
    };
}

namespace la
{

    ParametricState::ParametricState(State *state)
    {

        std::vector<Position*> stateMemberPositionArray = state->getMemberPositions();
        int memberPositionCount = stateMemberPositionArray.size();
        memberPositionBoundaryOffsetArray.resize(memberPositionCount);
        memberPositionEArray.resize(memberPositionCount);
        memberPositionTArray.resize(memberPositionCount);

        for (int i = 0; i < memberPositionCount; i++)
        {
            memberPositionBoundaryOffsetArray[i] = stateMemberPositionArray[i]->getI() - stateMemberPositionArray[0]->getI();
            memberPositionEArray[i] = stateMemberPositionArray[i]->getE();
            memberPositionTArray[i] = stateMemberPositionArray[i]->getT();
        }
        /////
    }

    ParametricState::ParametricState(State *state, int transitionBoundaryOffset)
            : ParametricState{state},
            transitionBoundaryOffset{transitionBoundaryOffset} {}

    int ParametricState::getLargestPositionOffset()
    {
        return memberPositionBoundaryOffsetArray[memberPositionBoundaryOffsetArray.size() - 1];
    }

    int ParametricState::getTransitionBoundaryOffset()
    {
        return transitionBoundaryOffset;
    }

    State *ParametricState::createActualState(int minimalBoundary)
    {
        int memberPositionCount = memberPositionBoundaryOffsetArray.size();
        std::vector<Position*> actualStateMemberPositionArray(memberPositionCount);
        for (int i = 0; i < memberPositionCount; i++)
        {
            int currentBoundaryOffset = memberPositionBoundaryOffsetArray[i];
            int currentE = memberPositionEArray[i];
            bool currentT = memberPositionTArray[i];

            actualStateMemberPositionArray[i] = new Position(minimalBoundary + currentBoundaryOffset, currentE, currentT);
        }
        return new State(actualStateMemberPositionArray);
    }

    bool ParametricState::equals(ParametricState *obj)
    {
        bool areEqual = (this == obj);
//        相等返回0,大于返回1，小于返回-1
        if (!areEqual && obj != nullptr)
        {
            ParametricState *pState{obj};
            areEqual = (this->memberPositionBoundaryOffsetArray == pState->memberPositionBoundaryOffsetArray) &&
                    (this->memberPositionEArray ==  pState->memberPositionEArray) &&
                    (this->memberPositionTArray == pState->memberPositionTArray);
        }

        return areEqual;
    }


    int ParametricState::hashCode()
    {
        int hash = 7;
        hash = 61 * hash + std::hash<std::vector<int>>()(this->memberPositionBoundaryOffsetArray);
        hash = 61 * hash + std::hash<std::vector<int>>()(this->memberPositionEArray);
        hash = 61 * hash + std::hash<std::vector<bool>>()(this->memberPositionTArray);
        return hash;
    }

    std::string ParametricState::toString()
    {
        std::string returnString = "i" + std::string (memberPositionTArray[0] ? "(t)" : "") + "#" + std::to_string(memberPositionEArray[0]) + ")";

        int memberPositionCount = memberPositionBoundaryOffsetArray.size();
        for (int i = 1; i < memberPositionCount; i++)
        {
            returnString += " (i + " + std::to_string(memberPositionBoundaryOffsetArray[i]) + ")" + (memberPositionTArray[i] ? "(t)" : "") + "(#" + std::to_string(memberPositionEArray[i]) + ")";
        }

        if (transitionBoundaryOffset != 0)
        {
            returnString.append(" "+ std::to_string(transitionBoundaryOffset));
        }

        return returnString;
    }


}
