#include "levenshtein_automaton/SimpleMDAGNode.h"

namespace la
{
    SimpleMDAGNode::SimpleMDAGNode(char letter, bool isAcceptNode, int transitionSetSize)
        : letter{letter},
        isAcceptNode{isAcceptNode},
        transitionSetSize{transitionSetSize},
        transitionSetBeginIndex{0} {}

    char SimpleMDAGNode::getLetter()
    {
        return this->letter;
    }

    bool SimpleMDAGNode::getIsAcceptNode()
    {
        return this->isAcceptNode;
    }

    int SimpleMDAGNode::getTransitionSetBeginIndex()
    {
        return this->transitionSetBeginIndex;
    }

    int SimpleMDAGNode::getOutgoingTransitionSetSize()
    {
        return this->transitionSetSize;
    }

    void SimpleMDAGNode::setTransitionSetBeginIndex(int transitionSetBeginIndex)
    {
        this->transitionSetBeginIndex = transitionSetBeginIndex;
    }

    SimpleMDAGNode *SimpleMDAGNode::transition(std::vector<SimpleMDAGNode*> &mdagDataArray, char letter)
    {
        int onePastTransitionSetEndIndex{this->transitionSetBeginIndex + this->transitionSetSize};
        SimpleMDAGNode *targetNode{nullptr};
        for(int i = this->transitionSetBeginIndex; i < onePastTransitionSetEndIndex; ++i)
        {
            if (mdagDataArray[i]->getLetter() == letter)
            {
                targetNode = mdagDataArray[i];
                break;
            }
        }
        return targetNode;
    }

    SimpleMDAGNode *SimpleMDAGNode::transition(std::vector<SimpleMDAGNode*> &mdagDataArray, const std::string &str)
    {
        SimpleMDAGNode *currentNode{this};
        int numberOfChars{static_cast<int>(str.length())};
        for(int i = 0; i < numberOfChars; ++i)
        {
            currentNode = currentNode->transition(mdagDataArray, str.at(i));
            if (currentNode == nullptr)
            {
                break;
            }
        }
        return currentNode;
    }

    SimpleMDAGNode *SimpleMDAGNode ::traverseMDAG(std::vector<SimpleMDAGNode*> &mdagDataArray, SimpleMDAGNode *sourceNode, const std::string &str)
    {
        char firstLetter{str.at(0)};
        for(int i = 0; i < sourceNode->transitionSetSize; ++i)
        {
            if (mdagDataArray[i]->getLetter() == firstLetter)
            {
                return mdagDataArray[i]->transition(mdagDataArray, str.substr(1));
            }
        }
        return nullptr;
    }
}