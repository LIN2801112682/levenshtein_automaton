#pragma once

#include <vector>
#include <string>

namespace la
{
    class SimpleMDAGNode
    {
    private:
        char letter;
        bool isacceptNode;
        int transitionSetSize;
        int transitionSetBeginIndex = 0;

    public:
        SimpleMDAGNode(char letter, bool isAcceptNode, int transitionSetSize);
        char getLetter();
        bool isAcceptNode();
        int getTransitionSetBeginIndex();
        int getOutgoingTransitionSetSize();
        void setTransitionSetBeginIndex(int transitionSetBeginIndex);
        SimpleMDAGNode *transition(std::vector<SimpleMDAGNode *> &mdagDataArray, char letter);
        SimpleMDAGNode *transition(std::vector<SimpleMDAGNode *> &mdagDataArray, const std::string &str);
        static SimpleMDAGNode *traverseMDAG(std::vector<SimpleMDAGNode *> &mdagDataArray, SimpleMDAGNode *sourceNode, const std::string &str);
    };
}