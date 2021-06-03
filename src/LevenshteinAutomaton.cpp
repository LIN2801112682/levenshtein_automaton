#include "../include/LevenshteinAutomaton.h"
#include "../include/AugBitSet.h"
#include "../include/ParametricState.h"
#include "../include/State.h"
#include "../include/MDAG.h"
#include "../include/SimpleMDAGNode.h"
#include "../include/MDAGNode.h"

namespace la
{

    const std::unordered_map<int, std::unordered_map<ParametricState*, std::unordered_map<AugBitSet*, ParametricState*>>> LevenshteinAutomaton::transitionHashMapContainerHashMap = std::unordered_map<int, std::unordered_map<ParametricState*, std::unordered_map<AugBitSet*, ParametricState*>>>();
    const State initialState = std::State(std::vector<Position *>={new Position(0,0,false)})

    LevenshteinAutomaton::LevenshteinAutomaton()
    {
    }

    std::vector<void*> LevenshteinAutomaton::createProcessingStepStackEntry(std::string &transitionPathString, void *mdagTransitionPathEndNode, State *lAutomatonTransitionPathEndState)
    {
        std::vector<void*> processingStepDataArray(4);
        processingStepDataArray[0] = transitionPathString;
        processingStepDataArray[1] = mdagTransitionPathEndNode;
        processingStepDataArray[2] = lAutomatonTransitionPathEndState;

        return processingStepDataArray;
    }

    std::list<std::string> LevenshteinAutomaton::iterativeFuzzySearch(int maxEditDistance, std::string &automatonString, MDAG *mdag)
    {
        std::list<std::string> resultStringLinkedList;
        std::stack<std::vector<void*>> processingStepStack;
        processingStepStack.push(createProcessingStepStackEntry("", mdag->getSourceNode(), initialState));
        std::unordered_set<char> charTreeSet = mdag->getTransitionLabelSet();
        int charCount = charTreeSet.size();
        int counter = 0;
        char charArray[charCount];
        for(char c : charTreeSet)
            charArray[counter++] = c;
        int j = 0;
        while(!processingStepStack.empty())
        {
            SMI currentProcessingStepDataArray = processingStepStack.pop();

            MDAGNode *currentNodeObj = currentProcessingStepDataArray[1];
            State *currentState = currentProcessingStepDataArray[2];
            j++;
            for(int i = 0; i < charCount; i++)
            {
                char currentChar = charArray[i];
                MDAGNode *transitionNode = (currentNodeObj)->transition(currentChar);
                if(transitionNode != nullptr)
                {
                    State *transitionState = currentState->transition(maxEditDistance, automatonString, currentChar);
                    if(transitionState != nullptr)
                    {
                        std::string transitionPathString = currentProcessingStepDataArray[0] + currentChar;
                        processingStepStack.push(createProcessingStepStackEntry(transitionPathString, transitionNode, transitionState));
                        if(MDAG->isAcceptNode(transitionNode) && isAcceptState(transitionState, automatonString.length(), maxEditDistance))
                            resultStringLinkedList.push_back(transitionPathString);
                    }
                }
            }
        }
        return resultStringLinkedList;
    }

    bool LevenshteinAutomaton::isAcceptingPosition(Position *p, int automatonStringLength, int automatonMaxEditDistance)
    {
        return (automatonStringLength - p->getI() <= automatonMaxEditDistance - p->getE());
    }

    bool LevenshteinAutomaton::isAcceptState(State *s, int automatonStringLength, int automatonMaxEditDistance)
    {
        for (auto p : s->getMemberPositions())
        {
            if (isAcceptingPosition(p, automatonStringLength, automatonMaxEditDistance))
            {
                return true;
            }
        }

        return false;
    }

}
