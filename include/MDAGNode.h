#pragma once

#include <string>
#include <stack>
#include <unordered_map>
#include <map>

namespace la
{
    class MDAGNode{
    private:
        bool isaccpetNode = false;
        std::map<char, MDAGNode*> outgoingTransitionTreeMap;
        int incomingTransitionCount = 0;
        int transitionSetBeginIndex = -1;
        int storedHashCode = -1;
        MDAGNode(MDAGNode &node);

    public:
        MDAGNode(bool isAcceptNode);
        MDAGNode *clone();
        MDAGNode *clone(MDAGNode *soleParentNode, char parentToCloneTransitionLabelChar);
        int getTransitionSetBeginIndex();
        int getOutgoingTransitionCount();
        int getIncomingTransitionCount();
        bool isConfluenceNode();
        bool isAcceptNode();
        void setAcceptStateStatus(bool isAcceptNode);
        void setTransitionSetBeginIndex(int transitionSetBeginIndex);
        bool hasOutgoingTransition(char letter);
        bool hasTransitions();
        MDAGNode *transition(char letter);
        MDAGNode *transition(std::string &str);
        std::stack<MDAGNode*> getTransitionPathNodes(std::string &str);
        std::map<char, MDAGNode*> getOutgoingTransitions();
        void decrementTargetIncomingTransitionCounts();
        void reassignOutgoingTransition(char letter, MDAGNode *oldTargetNode, MDAGNode *newTargetNode);
        MDAGNode *addOutgoingTransition(char letter, bool targetAcceptStateStatus);
        void removeOutgoingTransition(char letter);
        static bool haveSameTransitions(MDAGNode *node1, MDAGNode *node2);//
        void clearStoredHashCode();
        bool equals(void *obj);//
        int hashCode();
    };
}