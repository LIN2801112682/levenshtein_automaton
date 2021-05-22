#include "../include/State.h"
#include "../include/Position.h"
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <array>
namespace la
{
    State::State(State *s, Position *p)
    {
        std::vector<Position*> memberPositionArray = s->memberPositionArray;
        int newMemberPositionArrayLength = s->memberPositionArray.size() + 1;
        memberPositionArray[newMemberPositionArrayLength - 1] = p;
        std::sort(memberPositionArray.begin(),memberPositionArray.end());
    }
//    State::State(std::unordered_set<State *> &stateCollection, int &maxEditDistance){};
    State::State(std::vector<State*> &stateCollection, int &maxEditDistance)
    {
//        prospectiveMemberPositionCollection::toArray(std::vector<Position*>(prospectiveMemberPositionCollection->size())))
        std::unordered_set<Position*> prospectiveMemberPositionCollection;
        for (auto state : stateCollection)
//        for (auto it = stateCollection.begin();it != stateCollection.end(); it++)
        {
            for(auto position:memberPositionArray){
                prospectiveMemberPositionCollection.insert(position);
            }
        }

        std::unordered_set<Position*>::const_iterator it1 = prospectiveMemberPositionCollection.begin();
        while (it1 != prospectiveMemberPositionCollection.end())
        {
            Position position1;
            position1 = (Position) *it1;
            std::unordered_set<Position*>::const_iterator it2 = prospectiveMemberPositionCollection.begin();
            while (it2 != prospectiveMemberPositionCollection.end())
            {
                Position position2 = (Position)*it2;

                if (position2.subsumes(position1, maxEditDistance))
                {
                    (*it1)->remove();
                    break;
                }
                it2++;
            }
            it1++;
        }
        std::sort(memberPositionArray.begin(),memberPositionArray.end());
    }

    int State::getMinimalBoundary()
    {
        return memberPositionArray[0]->getI();
    }

    std::vector<Position*> State::getMemberPositions()
    {
        return memberPositionArray;
    }

    AugBitSet *State::getRelevantSubwordCharacteristicVector(int maxEditDistance, const std::wstring &automatonString, wchar_t letter)
    {
        //Isolate the substring in automatonString that serves as this State's relevant subword
        int minimalPositionBoundary = memberPositionArray[0]->getI();
        int relevantSubwordSize = std::min(2 * maxEditDistance + 1, (int)(automatonString.length() - minimalPositionBoundary));
        std::wstring relevantSubword = automatonString.substr(minimalPositionBoundary, relevantSubwordSize);
        AugBitSet *relevantSubwordCharacteristicVector = new AugBitSet(relevantSubwordSize);
        for (int i = 0; i < relevantSubwordSize; i++)
        {
            if (relevantSubword[i] == letter)
            {
                relevantSubwordCharacteristicVector->set(i);
            }
        }
        /////

        return relevantSubwordCharacteristicVector;
    }

    State *State::transition(int maxEditDistance, AugBitSet *relevantSubwordCharacteristicVector)
    {
        std::unordered_set<State *> newStateHashSet;
        int boundaryOffset = memberPositionArray[0]->getI();
        for (auto p : memberPositionArray)
        {
            State *transitionState = p->transition(maxEditDistance, p->getI() - boundaryOffset, relevantSubwordCharacteristicVector);
            if (transitionState != nullptr)
            {
                newStateHashSet.insert(transitionState);
            }
        }
        State tempVar = State(newStateHashSet, maxEditDistance);
        return (newStateHashSet.empty() ? nullptr : &tempVar);
    }

    State *State::transition(int maxEditDistance, const std::wstring &automatonString, wchar_t letter)
    {
        return transition(maxEditDistance, getRelevantSubwordCharacteristicVector(maxEditDistance, automatonString, letter));
    }

    std::string State::toString()
    {
        std::string returnString = memberPositionArray[0]->toString();

        int memberPositionCount = memberPositionArray.size();
        for (int i = 1; i < memberPositionCount; i++)
        {
//            returnString.append(" ");
//            returnString.append(memberPositionArray[i]->toString());
              returnString += " " + memberPositionArray[i]->toString();
        }

        return returnString;
    }

    bool State::canBeState(State *state, Position *prospectivePosition, int maxEditDistance)
    {
        for (auto currentMemberPosition : state->memberPositionArray)
        {
            if (currentMemberPosition->subsumes(prospectivePosition, maxEditDistance))
            {
                return false;
            }
        }

        return true;
    }

    int State::getMinimumBoundariesDifference(State *state1, State *state2)
    {
        return std::abs(state1->memberPositionArray[0]->getI() - state2->memberPositionArray[0]->getI());
    }
}
