#pragma once

#include <map>
#include <stack>
#include <string>

namespace la
{
    class MDAGNode{
    private:
        bool isAcceptNode;
        std::map<char, MDAGNode *> outgoingTransitionTreeMap;
        int incomingTransitionCount{0};
        int transitionSetBeginIndex{-1};
        // todo nullptr
        int storedHashCode{0};

    public:
        MDAGNode(bool isAcceptNode);
        MDAGNode *clone();
        MDAGNode *clone(MDAGNode *soleParentNode, char parentToCloneTransitionLabelChar);
        int getTransitionSetBeginIndex();
        int getOutgoingTransitionCount();
        int getIncomingTransitionCount();
        bool isConfluenceNode();
        bool getIsAcceptNode();
        void setAcceptStateStatus(bool isAcceptNode);
        void setTransitionSetBeginIndex(int transitionSetBeginIndex);
        bool hasOutgoingTransition(char letter);
        bool hasTransitions();
        MDAGNode *transition(char letter);
        MDAGNode *transition(const std::string &str);
        std::stack<MDAGNode *> getTransitionPathNodes(const std::string &str);
        std::map<char, MDAGNode *> getOutgoingTransitions();
        void decrementTargetIncomingTransitionCounts();
        void reassignOutgoingTransition(char letter, MDAGNode *oldTargetNode, MDAGNode *newTargetNode);
        MDAGNode *addOutgoingTransition(char letter, bool targetAcceptStateStatus);
        void removeOutgoingTransition(char letter);
        static bool haveSameTransitions(const MDAGNode *node1, const MDAGNode *node2);
        void clearStoredHashCode();
        int hashCode();
        bool equals(const MDAGNode &obj);
    private:
        MDAGNode(const MDAGNode &node);
    };
}