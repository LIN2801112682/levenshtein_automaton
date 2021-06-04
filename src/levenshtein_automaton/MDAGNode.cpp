#include "levenshtein_automaton/MDAGNode.h"

namespace la
{
    MDAGNode::MDAGNode(bool isAcceptNode)
        : isAcceptNode{isAcceptNode}
    {}

    MDAGNode::MDAGNode(const MDAGNode &node)
        : isAcceptNode{node.isAcceptNode},
        outgoingTransitionTreeMap{node.outgoingTransitionTreeMap}
    {
        for (auto &transitionKeyValuePair : this->outgoingTransitionTreeMap) {
            ++(transitionKeyValuePair.second->incomingTransitionCount);
        }
    }

    MDAGNode *
    MDAGNode::clone()
    {
        return new MDAGNode(*this);
    }

    MDAGNode *
    MDAGNode::clone(MDAGNode *soleParentNode, char parentToCloneTransitionLabelChar)
    {
        MDAGNode *cloneNode = new MDAGNode(*this);
        soleParentNode->reassignOutgoingTransition(parentToCloneTransitionLabelChar, this, cloneNode);
        return cloneNode;
    }

    int 
    MDAGNode::getTransitionSetBeginIndex()
    {
        return this->transitionSetBeginIndex;
    }

    int 
    MDAGNode::getOutgoingTransitionCount()
    {
        return this->outgoingTransitionTreeMap.size();
    }

    int 
    MDAGNode::getIncomingTransitionCount()
    {
        return this->incomingTransitionCount;
    }

    bool
    MDAGNode::isConfluenceNode()
    {
        return this->incomingTransitionCount > 1;
    }

    bool
    MDAGNode::getIsAcceptNode()
    {
        return this->isAcceptNode;
    }

    void 
    MDAGNode::setAcceptStateStatus(bool isAcceptNode)
    {
        this->isAcceptNode = isAcceptNode;
    }

    void
    MDAGNode::setTransitionSetBeginIndex(int transitionSetBeginIndex)
    {
        this->transitionSetBeginIndex = transitionSetBeginIndex;
    }

    bool
    MDAGNode::hasOutgoingTransition(char letter)
    {
        return this->outgoingTransitionTreeMap.contains(letter);
    }

    bool
    MDAGNode::hasTransitions()
    {
        return !this->outgoingTransitionTreeMap.empty();
    }

    MDAGNode *
    MDAGNode::transition(char letter)
    {
        return this->outgoingTransitionTreeMap.at(letter);
    }

    MDAGNode *
    MDAGNode::transition(const std::string &str)
    {
        int charCount{static_cast<int>(str.length())};
        MDAGNode *currentNode{this};
        for (int i{0}; i < charCount; ++i)
        {
            currentNode = currentNode->transition(str.at(i));
            if (currentNode == nullptr)
                break;
        }
        return currentNode;
    }

    std::stack<MDAGNode *>
    MDAGNode::getTransitionPathNodes(const std::string &str)
    {
        std::stack<MDAGNode *> nodeStack;
        MDAGNode *currentNode{this};
        int numberOfChars{static_cast<int>(str.length())};
        for (int i{0}; i < numberOfChars && currentNode != nullptr; ++i)
        {
            currentNode = currentNode->transition(str.at(i));
            nodeStack.push(currentNode);
        }
        return nodeStack;
    }

    std::map<char, MDAGNode *>
    MDAGNode::getOutgoingTransitions()
    {
        return this->outgoingTransitionTreeMap;
    }

    void
    MDAGNode::decrementTargetIncomingTransitionCounts()
    {
        for (auto &transitionKeyValuePair : this->outgoingTransitionTreeMap)
        {
            --(transitionKeyValuePair.second->incomingTransitionCount);
        }
    }

    void
    MDAGNode::reassignOutgoingTransition(char letter, MDAGNode *oldTargetNode, MDAGNode *newTargetNode)
    {
        --oldTargetNode->incomingTransitionCount;
        ++newTargetNode->incomingTransitionCount;
        this->outgoingTransitionTreeMap.emplace(letter, newTargetNode);
    }

    MDAGNode *
    MDAGNode::addOutgoingTransition(char letter, bool targetAcceptStateStatus)
    {
        MDAGNode *newTargetNode = new MDAGNode(targetAcceptStateStatus);
        ++newTargetNode->incomingTransitionCount;
        this->outgoingTransitionTreeMap.emplace(letter, newTargetNode);
        return newTargetNode;
    }

    void MDAGNode::removeOutgoingTransition(char letter)
    {
        this->outgoingTransitionTreeMap.erase(letter);
    }

    bool MDAGNode::haveSameTransitions(const MDAGNode *node1, const MDAGNode *node2)
    {
        std::map<char, MDAGNode *> outgoingTransitionTreeMap1 = node1->outgoingTransitionTreeMap;
        std::map<char, MDAGNode *> outgoingTransitionTreeMap2 = node2->outgoingTransitionTreeMap;
        if (outgoingTransitionTreeMap1.size() != outgoingTransitionTreeMap2.size())
            return false;
        else
        {
            auto i$ = std::begin(outgoingTransitionTreeMap1);
            char currentCharKey;
            MDAGNode *currentTargetNode;
            do {
                if (i$ != std::end(outgoingTransitionTreeMap1))
                    return true;
                auto transitionKeyValuePair = *(++i$);
                currentCharKey = transitionKeyValuePair.first;
                currentTargetNode = transitionKeyValuePair.second;

                auto iter = outgoingTransitionTreeMap2.find(currentCharKey);
                if (iter == std::end(outgoingTransitionTreeMap2))
                    break;
                if (iter->second != currentTargetNode)
                    break;
            } while (true);
            return false;
        }
    }

    void MDAGNode::clearStoredHashCode()
    {
        // todo 0
        this->storedHashCode = 0;
    }

    bool MDAGNode::equals(const MDAGNode &obj)
    {
        bool areEqual = this == &obj;
        if (!areEqual)
        {
            areEqual = this->isAcceptNode == obj.isAcceptNode && haveSameTransitions(this, &obj);
        }
        return areEqual;
    }

    int MDAGNode::hashCode()
    {
        if (this->storedHashCode != -1)
        {
            int hash = 7;
            hash = 53 * hash + (this->isAcceptNode ? 1 : 0);
            // todo
            //hash = 53 * hash + std::hash<decltype(this->outgoingTransitionTreeMap)>{}(this->outgoingTransitionTreeMap>);
            this->storedHashCode = hash;
            return hash;
        }
        else
            return this->storedHashCode;
    }
}