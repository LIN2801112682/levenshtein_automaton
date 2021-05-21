#include <levenshtein_automaton/MDAGNode.h>
#include <levenshtein_automaton/SimpleMDAGNode.h>

namespace la
{
    SimpleMDAGNode::SimpleMDAGNode(char letter, boolean isAcceptNode, int transitionSetSize)
    {
        this->letter = letter;
        this->isacceptNode = isAcceptNode;
        this->transitionSetSize = transitionSetSize;
        this->transitionSetBeginIndex = 0;
    }

    char SimpleMDAGNode::getLetter()
    {
        return this->letter;
    }

    bool SimpleMDAGNode::isAcceptNode()
    {
        return this->isacceptNode;
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
        int onePastTransitionSetEndIndex = this->transitionSetBeginIndex + this->transitionSetSize;
        SimpleMDAGNode *targetNode = nullptr;
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

    SimpleMDAGNode *SimpleMDAGNode::transition(std::vector<SimpleMDAGNode*> &mdagDataArray, std::string &str)
    {
        SimpleMDAGNode *currentNode = this;
        int numberOfChars = str.length();
        for(int i = 0; i < numberOfChars; ++i)
        {
            currentNode = currentNode->transition(mdagDataArray, str[i]);
            if (currentNode == nullptr)
            {
                break;
            }
        }
        return currentNode;
    }

    SimpleMDAGNode *SimpleMDAGNode ::traverseMDAG(std::vector<SimpleMDAGNode*> &mdagDataArray, SimpleMDAGNode *sourceNode, std::string &str)
    {
        char firstLetter = str[0];
        for(int i = 0; i < sourceNode->transitionSetSize; ++i)
        {
            if (mdagDataArray[i]->getLetter() == firstLetter)
            {
                std::string str_new = str.substr(1);
                return mdagDataArray[i]->transition(mdagDataArray, str_new);
            }
        }
        return nullptr;
    }
}