#include "../include/SimpleMDAGNode.h"
#include "../include/MDAGNode.h"

namespace la
{
    MDAGNode::MDAGNode(bool isAcceptNode)
    {
        this->isaccpetNode = isAcceptNode;
        //this->outgoingTransitionTreeMap = new std::map<char, MDAGNode*>;
    }
    MDAGNode::MDAGNode(MDAGNode &node)
    {
        this->isaccpetNode = node.isaccpetNode;
        this->outgoingTransitionTreeMap = node.outgoingTransitionTreeMap;
        /*Map.Entry transitionKeyValuePair;
        for (Iterator i$ = this.outgoingTransitionTreeMap.entrySet().iterator(); i$.hasNext(); ++((MDAGNode) transitionKeyValuePair.getValue()).incomingTransitionCount) {
            transitionKeyValuePair = (Map.Entry) i$.next();
        }*/
        for (auto &pair : this->outgoingTransitionTreeMap )
        {
            ++pair.second->incomingTransitionCount;
        }
    }
    MDAGNode *MDAGNode::clone()
    {
        return new MDAGNode(this);
    }
    MDAGNode *MDAGNode::clone(MDAGNode *soleParentNode, char parentToCloneTransitionLabelChar)
    {
        MDAGNode *cloneNode = new MDAGNode(this);
        soleParentNode->reassignOutgoingTransition(parentToCloneTransitionLabelChar, this, cloneNode);
        return cloneNode;
    }
    int MDAGNode::getTransitionSetBeginIndex()
    {
        return this->transitionSetBeginIndex;
    }
    int MDAGNode::getOutgoingTransitionCount()
    {
        return this->outgoingTransitionTreeMap.size();
    }
    int MDAGNode::getIncomingTransitionCount()
    {
        return this->incomingTransitionCount;
    }
    bool MDAGNode::isConfluenceNode()
    {
        return this->incomingTransitionCount > 1;
    }
    bool MDAGNode::isAcceptNode()
    {
        return this->isaccpetNode;
    }
    void MDAGNode::setAcceptStateStatus(bool isAcceptNode)
    {
        this->isaccpetNode = isAcceptNode;
    }
    void MDAGNode::setTransitionSetBeginIndex(int transitionSetBeginIndex)
    {
        this->transitionSetBeginIndex = transitionSetBeginIndex;
    }
    bool MDAGNode::hasOutgoingTransition(char letter)
    {
        return this->outgoingTransitionTreeMap.find(letter) != this->outgoingTransitionTreeMap.end();
    }
    bool MDAGNode::hasTransitions()
    {
        return !this->outgoingTransitionTreeMap.empty();
    }
    MDAGNode *MDAGNode::transition(char letter)
    {
        return (MDAGNode*) this->outgoingTransitionTreeMap.find(letter)->second;
    }
    MDAGNode *MDAGNode::transition(std::string &str)
    {
        int charCount = str.length();
        MDAGNode *currentNode = this;
        for (int i = 0; i < charCount; ++i)
        {
            currentNode = currentNode->transition(str[i]);
            if (currentNode == nullptr)
                break;
        }
        return currentNode;
    }
    std::stack<MDAGNode*> MDAGNode::getTransitionPathNodes(std::string &str)
    {
        std::stack<MDAGNode*> nodeStack;
        MDAGNode *currentNode = this;
        int numberOfChars = str.length();
        for (int i = 0; i < numberOfChars && currentNode != nullptr; ++i)
        {
            currentNode = currentNode->transition(str[i]);
            nodeStack.push(currentNode);
        }
        return nodeStack;
    }
    std::map<char, MDAGNode*> MDAGNode::getOutgoingTransitions()
    {
        return this->outgoingTransitionTreeMap;
    }
    void MDAGNode::decrementTargetIncomingTransitionCounts()
    {
        for (auto &pair : this->outgoingTransitionTreeMap )
        {
            --pair.second->incomingTransitionCount;
        }
    }
    void MDAGNode::reassignOutgoingTransition(char letter, MDAGNode *oldTargetNode, MDAGNode *newTargetNode)
    {
        --oldTargetNode->incomingTransitionCount;
        ++newTargetNode->incomingTransitionCount;
        //this->outgoingTransitionTreeMap.insert(letter,newTargetNode);
        this->outgoingTransitionTreeMap[letter] = newTargetNode;
    }

    MDAGNode * MDAGNode::addOutgoingTransition(char letter, bool targetAcceptStateStatus)
    {
        MDAGNode *newTargetNode = new MDAGNode(targetAcceptStateStatus);
        ++newTargetNode->incomingTransitionCount;
        //this->outgoingTransitionTreeMap.insert(letter, newTargetNode);
        this->outgoingTransitionTreeMap[letter] = newTargetNode;
        return newTargetNode;
    }
    void MDAGNode::removeOutgoingTransition(char letter)
    {
        this->outgoingTransitionTreeMap.erase(letter);
    }
    bool MDAGNode::haveSameTransitions(MDAGNode *node1, MDAGNode *node2)
    {
        std::map<char, MDAGNode*> outgoingTransitionTreeMap1 = node1->outgoingTransitionTreeMap;
        std::map<char, MDAGNode*> outgoingTransitionTreeMap2 = node2->outgoingTransitionTreeMap;
        if (outgoingTransitionTreeMap1.size() != outgoingTransitionTreeMap2.size())
            return false;
        else
        {
            auto i$ = outgoingTransitionTreeMap1.begin();
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
        this->storedHashCode = -1;
    }
    bool MDAGNode::equals(void *obj)
    {
        bool areEqual = this == obj;
        if (!areEqual && obj != nullptr)
        {
            MDAGNode *node = (MDAGNode*) obj;
            areEqual = this->isaccpetNode == node->isaccpetNode && haveSameTransitions(this, node);
        }
        return areEqual;
    }
    int MDAGNode::hashCode()
    {
        if (this->storedHashCode != -1)
        {
            int hash = 7;
            hash = 53 * hash + (this->isaccpetNode ? 1 : 0);
//            hash = 53 * hash + (this->outgoingTransitionTreeMap.size()>0 ? this->outgoingTransitionTreeMap.hashCode() : 0);
            this->storedHashCode = hash;
            return hash;
        }
        else
            return this->storedHashCode;
    }
}