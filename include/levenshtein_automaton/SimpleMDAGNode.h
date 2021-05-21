#pragma once

#include <rpcndr.h>
#include <string>
#include <vector>

namespace la
{
    class SimpleMDAGNode{
    private:
        char letter;
        bool isacceptNode;
        int transitionSetSize;
        int transitionSetBeginIndex = 0;

    public:
        SimpleMDAGNode(char letter, boolean isAcceptNode, int transitionSetSize);
        char getLetter();
        bool isAcceptNode();
        int getTransitionSetBeginIndex();
        int getOutgoingTransitionSetSize();
        void setTransitionSetBeginIndex(int transitionSetBeginIndex);
        SimpleMDAGNode *transition(std::vector<SimpleMDAGNode*> &mdagDataArray, char letter);
        SimpleMDAGNode *transition(std::vector<SimpleMDAGNode*> &mdagDataArray, std::string &str);
        static SimpleMDAGNode *traverseMDAG(std::vector<SimpleMDAGNode*> &mdagDataArray, SimpleMDAGNode *sourceNode, std::string &str);
    };
}