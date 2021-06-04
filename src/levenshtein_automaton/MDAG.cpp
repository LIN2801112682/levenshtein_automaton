#include "levenshtein_automaton/MDAG.h"

namespace la
{
    MDAG::MDAG(const std::vector<std::string> &strCollection)
    {
        this->addStrings(strCollection);
    }
    
    void
    MDAG::addStrings(const std::vector<std::string> &strCollection)
    {
        if (this->sourceNode != nullptr)
        {
            std::string previousString{""};
            for (const auto &currentString : strCollection)
            {
                int mpsIndex{this->calculateMinimizationProcessingStartIndex(previousString, currentString)};
                if (mpsIndex != -1)
                {
                    std::string transitionSubstring{previousString.substr(0, mpsIndex)};
                    std::string minimizationProcessingSubString{previousString.substr(mpsIndex)};
                    this->replaceOrRegister(this->sourceNode->transition(transitionSubstring), minimizationProcessingSubString);
                }
                this->addStringInternal(currentString);
                previousString = currentString;
            }
            this->replaceOrRegister(this->sourceNode, previousString);
        } else {
            // todo
            // throw ex
        }
    }

    int
    MDAG::calculateMinimizationProcessingStartIndex(const std::string &prevStr, const std::string &currStr)
    {
        int mpsIndex;
        if (!currStr.starts_with(prevStr)){
            int shortestStringLength{static_cast<int>(std::min(prevStr.length(), currStr.length()))};
            for (mpsIndex = 0; mpsIndex < shortestStringLength && prevStr.at(mpsIndex) == currStr.at(mpsIndex); ++mpsIndex);
        }
        else {
            mpsIndex = -1;
        }
        return mpsIndex;
    }

    std::string
    MDAG::determineLongestPrefixInMDAG(const std::string &str)
    {
        MDAGNode *currentNode{this->sourceNode};
        int numberOfChars{static_cast<int>(str.length())};
        int onePastPrefixEndIndex{0};
        for (int i{0}; i < numberOfChars; ++onePastPrefixEndIndex)
        {
            char currentChar = str.at(i);
            if (!currentNode->hasOutgoingTransition(currentChar))
                break;
            currentNode = currentNode->transition(currentChar);
            ++i;
        }
        return str.substr(0, onePastPrefixEndIndex);
    }

    std::tuple<int, MDAGNode *>
    MDAG::getTransitionPathFirstConfluenceNodeData(MDAGNode *originNode, const std::string &str)
    {
        int currentIndex{0};
        int charCount{static_cast<int>(str.length())};
        MDAGNode *currentNode;
        for (currentNode = originNode; currentIndex < charCount; ++currentIndex)
        {
            char currentChar{str.at(currentIndex)};
            currentNode = currentNode->hasOutgoingTransition(currentChar) ? currentNode->transition(currentChar) : nullptr;
            if (currentNode == nullptr || currentNode->isConfluenceNode())
                break;
        }
        bool noConfluenceNode = currentNode == originNode || currentIndex == charCount;
        if (noConfluenceNode) {
            return {0, nullptr};
        } else {
            return {currentIndex, currentNode};
        }
    }

    void
    MDAG::replaceOrRegister(MDAGNode *originNode, const std::string &str)
    {
        // add
        if (str.empty())
            return;
        char transitionLabelChar{str.at(0)};
        MDAGNode *relevantTargetNode{originNode->transition(transitionLabelChar)};
        if (relevantTargetNode->hasTransitions() && !str.substr(1).empty())
        {
            this->replaceOrRegister(relevantTargetNode, str.substr(1));
        }
        MDAGNode *equivalentNode = this->equivalenceClassMDAGNodeHashMap.at(relevantTargetNode);
        if (equivalentNode == nullptr)
            this->equivalenceClassMDAGNodeHashMap.emplace(relevantTargetNode, relevantTargetNode);
        else if (equivalentNode != relevantTargetNode)
        {
            relevantTargetNode->decrementTargetIncomingTransitionCounts();
            this->transitionCount -= relevantTargetNode->getOutgoingTransitionCount();
            originNode->reassignOutgoingTransition(transitionLabelChar, relevantTargetNode, equivalentNode);
        }
    }

    void
    MDAG::addTransitionPath(MDAGNode *originNode, const std::string &str)
    {
        if (!str.empty()) {
            MDAGNode *currentNode{originNode};
            int charCount{static_cast<int>(str.length())};
            for (int i = 0; i < charCount; ++this->transitionCount) {
                char currentChar = str.at(i);
                bool isLastChar = i == charCount - 1;
                currentNode = currentNode->addOutgoingTransition(currentChar, isLastChar);
                this->charTreeSet.insert(currentChar);
                ++i;
            }
        }
        else {
            originNode->setAcceptStateStatus(true);
        }
    }

    void
    MDAG::removeTransitionPathRegisterEntries(const std::string &str)
    {
        MDAGNode *currentNode{this->sourceNode};
        int charCount{static_cast<int>(str.length())};
        for (int i = 0; i < charCount; ++i)
        {
            currentNode = currentNode->transition(str.at(i));
            if (this->equivalenceClassMDAGNodeHashMap.at(currentNode) == currentNode) {
                this->equivalenceClassMDAGNodeHashMap.erase(currentNode);
            }
            currentNode->clearStoredHashCode();
        }
    }

    void
    MDAG::cloneTransitionPath(MDAGNode *pivotConfluenceNode, const std::string &transitionStringToPivotNode, const std::string &str)
    {
        MDAGNode *lastTargetNode = pivotConfluenceNode->transition(str);
        MDAGNode *lastClonedNode{nullptr};
        char lastTransitionLabelChar{0};
        for (int i = str.length(); i >= 0; --i)
        {
            std::string currentTransitionString = i > 0 ? str.substr(0, i) : "";
            MDAGNode *currentTargetNode = i > 0 ? pivotConfluenceNode->transition(currentTransitionString) : pivotConfluenceNode;
            MDAGNode *clonedNode;
            if (i == 0) {
                std::string transitionStringToPivotNodeParent{transitionStringToPivotNode.substr(0, transitionStringToPivotNode.length() - 1)};
                char parentTransitionLabelChar{transitionStringToPivotNode.at(transitionStringToPivotNode.length() - 1)};
                clonedNode = pivotConfluenceNode->clone(this->sourceNode->transition(transitionStringToPivotNodeParent), parentTransitionLabelChar);
            }
            else {
                clonedNode = currentTargetNode->clone();
            }
            this->transitionCount += clonedNode->getOutgoingTransitionCount();
            if (lastClonedNode != nullptr)
            {
                clonedNode->reassignOutgoingTransition(lastTransitionLabelChar, lastTargetNode, lastClonedNode);
                lastTargetNode = currentTargetNode;
            }
            lastClonedNode = clonedNode;
            lastTransitionLabelChar = i > 0 ? str.at(i - 1) : 0;
        }
    }

    void
    MDAG::addStringInternal(const std::string &str)
    {
        std::string prefixString{this->determineLongestPrefixInMDAG(str)};//前缀字符串
        std::string suffixString{str.substr(prefixString.length())};//后缀字符串
        auto [toFirstConfluenceNodeTransitionCharIndex, firstConfluenceNodeInPrefix] = this->getTransitionPathFirstConfluenceNodeData(this->sourceNode, prefixString);
        // todo 0
        this->removeTransitionPathRegisterEntries(toFirstConfluenceNodeTransitionCharIndex == 0 ? prefixString : prefixString.substr(0, toFirstConfluenceNodeTransitionCharIndex));
        if (firstConfluenceNodeInPrefix != nullptr)
        {
            std::string transitionStringOfPathToFirstConfluenceNode{prefixString.substr(0, toFirstConfluenceNodeTransitionCharIndex + 1)};
            std::string transitionStringOfToBeDuplicatedPath{prefixString.substr(toFirstConfluenceNodeTransitionCharIndex + 1)};
            this->cloneTransitionPath(firstConfluenceNodeInPrefix, transitionStringOfPathToFirstConfluenceNode, transitionStringOfToBeDuplicatedPath);
        }
        this->addTransitionPath(this->sourceNode->transition(prefixString), suffixString);
    }
}