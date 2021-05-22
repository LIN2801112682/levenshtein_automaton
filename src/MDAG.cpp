#include "../include/MDAG.h"
#include "../include/SimpleMDAGNode.h"
namespace la
{
    MDAG::MDAG(char *filename)
    {
        //BufferedReader dataFileBufferedReader = new BufferedReader(new FileReader(dataFile));
        std::fstream dataFileBufferedReader(filename);
        std::string currentString = "";
        char *str[256];
        std::string previousString;
        for (previousString = ""; (currentString = dataFileBufferedReader.getline(str,256)) != nullptr; previousString = currentString)
        {
            int mpsIndex = this->calculateMinimizationProcessingStartIndex(previousString, currentString);
            if (mpsIndex != -1)
            {
                std::string transitionSubstring = previousString.substr(0, mpsIndex);
                std::string minimizationProcessingSubstring = previousString.substr(mpsIndex);
                this->replaceOrRegister(this->sourceNode->transition(transitionSubstring), minimizationProcessingSubstring);
            }
                this->addStringInternal(currentString);
        }
        this->replaceOrRegister(this->sourceNode, previousString);
    }
    MDAG::MDAG(std::vector<std::string> *strCollection)
    {
        this->addStrings(strCollection);
    }
    void MDAG::addStrings(std::vector<std::string> *strCollection)
    {
        if (this->sourceNode != nullptr)
        {
            std::string previousString = "";
            std::string currentString;
            for (auto &pair : *strCollection)
            {
                currentString = (std::string) pair;
                int mpsIndex = this->calculateMinimizationProcessingStartIndex(previousString, currentString);
                if (mpsIndex != -1)
                {
                    std::string transitionSubstring = previousString.substr(0, mpsIndex);
                    std::string minimizationProcessingSubString = previousString.substr(mpsIndex);
                    this->replaceOrRegister(this->sourceNode->transition(transitionSubstring), minimizationProcessingSubString);
                }
                this->addStringInternal(currentString);
                previousString = currentString;
            }
            /*for (Iterator i$ = strCollection.iterator(); i$.hasNext(); previousString = currentString)
            {
                currentString = (std::string) i$.next();
                int mpsIndex = this->calculateMinimizationProcessingStartIndex(previousString, currentString);
                if (mpsIndex != -1)
                {
                    std::string transitionSubstring = previousString.substr(0, mpsIndex);
                    std::string minimizationProcessingSubString = previousString.substr(mpsIndex);
                    this->replaceOrRegister(this.sourceNode.transition(transitionSubstring), minimizationProcessingSubString);
                }
                this->addStringInternal(currentString);
            }*/
            this->replaceOrRegister(this->sourceNode, previousString);
        }
    }
    void MDAG::addString(std::string &str)
    {
        if (this->sourceNode)
        {
            this->addStringInternal(str);
            this->replaceOrRegister(this->sourceNode, str);
        }
    }
    void MDAG::splitTransitionPath(MDAGNode *originNode, std::string &storedStringSubstr)
    {
        std::unordered_map<std::string, Object> firstConfluenceNodeDataHashMap = this->getTransitionPathFirstConfluenceNodeData(originNode, storedStringSubstr);
        int toFirstConfluenceNodeTransitionCharIndex = (int) firstConfluenceNodeDataHashMap.get("toConfluenceNodeTransitionCharIndex");
        MDAGNode *firstConfluenceNode = (MDAGNode*) firstConfluenceNodeDataHashMap.get("confluenceNode");
        if (firstConfluenceNode != nullptr)
        {
            std::string str_new  = storedStringSubstr.substr(0, toFirstConfluenceNodeTransitionCharIndex);
            MDAGNode *firstConfluenceNodeParent = originNode->transition(str_new);
            MDAGNode *firstConfluenceNodeClone = firstConfluenceNode->clone(firstConfluenceNodeParent, storedStringSubstr[toFirstConfluenceNodeTransitionCharIndex]);
            this->transitionCount += firstConfluenceNodeClone->getOutgoingTransitionCount();
            std::string unprocessedSubString = storedStringSubstr.substr(toFirstConfluenceNodeTransitionCharIndex + 1);
            this->splitTransitionPath(firstConfluenceNodeClone, unprocessedSubString);
        }
    }
    int MDAG::calculateSoleTransitionPathLength(std::string &str)
    {
        std::stack<MDAGNode*> transitionPathNodeStack = this->sourceNode->getTransitionPathNodes(str);
        transitionPathNodeStack.pop();
        transitionPathNodeStack.trimToSize();
        while (!transitionPathNodeStack.empty())
        {
            MDAGNode currentNode = (MDAGNode) transitionPathNodeStack.top();
            if (currentNode.getOutgoingTransitions().size() > 1 || currentNode.isAcceptNode())
                break;
            transitionPathNodeStack.pop();
        }
        return transitionPathNodeStack.capacity() - transitionPathNodeStack.size();
    }
    void MDAG::removeString(std::string &str)
    {
        if (this->sourceNode != nullptr)
        {
            this->splitTransitionPath(this->sourceNode, str);
            this->removeTransitionPathRegisterEntries(str);
            MDAGNode strEndNode = this->sourceNode->transition(str);
            if (!strEndNode.hasTransitions())
            {
                int soleInternalTransitionPathLength = this->calculateSoleTransitionPathLength(str);
                int internalTransitionPathLength = str.length() - 1;
                if (soleInternalTransitionPathLength == internalTransitionPathLength)
                {
                    this->sourceNode->removeOutgoingTransition(str[0]);
                    this->transitionCount -= str.length();
                }
                else
                {
                    int toBeRemovedTransitionLabelCharIndex = internalTransitionPathLength - soleInternalTransitionPathLength;
                    std::string str_new1 = str.substr(0, toBeRemovedTransitionLabelCharIndex);
                    MDAGNode *latestNonSoloTransitionPathNode = this->sourceNode->transition(str_new1);
                    latestNonSoloTransitionPathNode->removeOutgoingTransition(str[toBeRemovedTransitionLabelCharIndex]);
                    this->transitionCount -= str.substr(toBeRemovedTransitionLabelCharIndex).length();
                    std::string str_new2 = str.substr(0, toBeRemovedTransitionLabelCharIndex);
                    this->replaceOrRegister(this->sourceNode, str_new2);
                }
            }
            else
            {
                strEndNode.setAcceptStateStatus(false);
                this->replaceOrRegister(this->sourceNode, str);
            }
        }
    }
    int MDAG::calculateMinimizationProcessingStartIndex(std::string &prevStr, std::string &currStr)
    {
        int mpsIndex;
        //if (!startsWith(currStr,prevStr))
        if (!currStr.compare(0,prevStr.size(),prevStr))
        {
            int shortestStringLength = std::min(prevStr.length(), currStr.length());
            for (mpsIndex = 0; mpsIndex < shortestStringLength && prevStr[mpsIndex] == currStr[mpsIndex]; ++mpsIndex)
            {}
        }
        else
        {
            mpsIndex = -1;
        }
        return mpsIndex;
    }
    std::string MDAG::determineLongestPrefixInMDAG(std::string &str)
    {
        MDAGNode *currentNode = this->sourceNode;
        int numberOfChars = str.length();
        int onePastPrefixEndIndex = 0;
        for (int i = 0; i < numberOfChars; ++onePastPrefixEndIndex)
        {
            char currentChar = str[i];
            if (!currentNode->hasOutgoingTransition(currentChar))
                break;
            currentNode = currentNode->transition(currentChar);
            ++i;
        }
        return str.substr(0, onePastPrefixEndIndex);
    }
    std::unordered_map<std::string, Object> MDAG::getTransitionPathFirstConfluenceNodeData(MDAGNode *originNode, std::string &str)
    {
        int currentIndex = 0;
        int charCount = str.length();
        MDAGNode *currentNode;
        for (currentNode = originNode; currentIndex < charCount; ++currentIndex)
        {
            char currentChar = str[currentIndex];
            currentNode = currentNode->hasOutgoingTransition(currentChar) ? currentNode->transition(currentChar) : nullptr;
            if (currentNode == nullptr || currentNode->isConfluenceNode())
                break;
        }
        boolean noConfluenceNode = currentNode == originNode || currentIndex == charCount;
        std::unordered_map<std::string, Object> confluenceNodeDataHashMap = new std::unordered_map(2);
        confluenceNodeDataHashMap.put("toConfluenceNodeTransitionCharIndex", noConfluenceNode ? nullptr : currentIndex);
        confluenceNodeDataHashMap.put("confluenceNode", noConfluenceNode ? nullptr : currentNode);
        return confluenceNodeDataHashMap;
    }
    void MDAG::replaceOrRegister(MDAGNode *originNode, std::string &str)
    {
        char transitionLabelChar = str[0];
        MDAGNode *relevantTargetNode = originNode->transition(transitionLabelChar);
        if (relevantTargetNode->hasTransitions() && !str.substr(1).empty())
        {
            std::string str_new = str.substr(1);
            this->replaceOrRegister(relevantTargetNode, str_new);
        }
        MDAGNode *equivalentNode = (MDAGNode*) this->equivalenceClassMDAGNodeHashMap[relevantTargetNode];
        if (equivalentNode == nullptr)
            this->equivalenceClassMDAGNodeHashMap.emplace(relevantTargetNode, relevantTargetNode);
        else if (equivalentNode != relevantTargetNode)
        {
            relevantTargetNode->decrementTargetIncomingTransitionCounts();
            this->transitionCount -= relevantTargetNode->getOutgoingTransitionCount();
            originNode->reassignOutgoingTransition(transitionLabelChar, relevantTargetNode, equivalentNode);
        }
    }
    void MDAG::addTransitionPath(MDAGNode *originNode, std::string &str)
    {
        if (!str.empty())
        {
            MDAGNode *currentNode = originNode;
            int charCount = str.length();
            for (int i = 0; i < charCount; ++this->transitionCount)
            {
                char currentChar = str[i];
                boolean isLastChar = i == charCount - 1;
                currentNode = currentNode->addOutgoingTransition(currentChar, isLastChar);
                this->charTreeSet.insert(currentChar);
                ++i;
            }
        }
        else
            originNode->setAcceptStateStatus(true);
    }
    void MDAG::removeTransitionPathRegisterEntries(std::string &str)
    {
        MDAGNode *currentNode = this->sourceNode;
        int charCount = str.length();
        for (int i = 0; i < charCount; ++i)
        {
            currentNode = currentNode->transition(str[i]);
            if (this->equivalenceClassMDAGNodeHashMap[currentNode] == currentNode)
                this->equivalenceClassMDAGNodeHashMap.erase(currentNode);
            currentNode->clearStoredHashCode();
        }
    }
    void MDAG::cloneTransitionPath(MDAGNode *pivotConfluenceNode, std::string &transitionStringToPivotNode, std::string &str)
    {
        MDAGNode *lastTargetNode = pivotConfluenceNode->transition(str);
        MDAGNode *lastClonedNode = nullptr;
        char lastTransitionLabelChar = 0;
        for (int i = str.length(); i >= 0; --i)
        {
            std::string currentTransitionString = i > 0 ? str.substr(0, i) : nullptr;
            MDAGNode *currentTargetNode = i > 0 ? pivotConfluenceNode->transition(currentTransitionString) : pivotConfluenceNode;
            MDAGNode *clonedNode;
            if (i == 0)
            {
                std::string transitionStringToPivotNodeParent = transitionStringToPivotNode.substr(0, transitionStringToPivotNode.length() - 1);
                char parentTransitionLabelChar = transitionStringToPivotNode[transitionStringToPivotNode.length() - 1];
                clonedNode = pivotConfluenceNode->clone(this->sourceNode->transition(transitionStringToPivotNodeParent), parentTransitionLabelChar);
            }
            else
                clonedNode = currentTargetNode->clone();
            this->transitionCount += clonedNode->getOutgoingTransitionCount();
            if (lastClonedNode != nullptr)
            {
                clonedNode->reassignOutgoingTransition(lastTransitionLabelChar, lastTargetNode, lastClonedNode);
                lastTargetNode = currentTargetNode;
            }
            lastClonedNode = clonedNode;
            lastTransitionLabelChar = i > 0 ? str[i - 1] : 0;
        }
    }

    void MDAG::addStringInternal(std::string &str)
    {
        std::string prefixString = this->determineLongestPrefixInMDAG(str);//前缀字符串
        std::string suffixString = str.substr(prefixString.length());//后缀字符串
        std::unordered_map<std::string, Object> firstConfluenceNodeDataHashMap = this->getTransitionPathFirstConfluenceNodeData(this->sourceNode, prefixString);
        MDAGNode *firstConfluenceNodeInPrefix = (MDAGNode*) firstConfluenceNodeDataHashMap.get("confluenceNode");
        int toFirstConfluenceNodeTransitionCharIndex = (int) firstConfluenceNodeDataHashMap.get("toConfluenceNodeTransitionCharIndex");
        this->removeTransitionPathRegisterEntries(toFirstConfluenceNodeTransitionCharIndex == nullptr ? prefixString : prefixString.substr(0, toFirstConfluenceNodeTransitionCharIndex));
        if (firstConfluenceNodeInPrefix != nullptr)
        {
            std::string transitionStringOfPathToFirstConfluenceNode = prefixString.substr(0, toFirstConfluenceNodeTransitionCharIndex + 1);
            std::string transitionStringOfToBeDuplicatedPath = prefixString.substr(toFirstConfluenceNodeTransitionCharIndex + 1);
            this->cloneTransitionPath(firstConfluenceNodeInPrefix, transitionStringOfPathToFirstConfluenceNode, transitionStringOfToBeDuplicatedPath);
        }
        this->addTransitionPath(this->sourceNode->transition(prefixString), suffixString);
    }
    int MDAG::createSimpleMDAGTransitionSet(MDAGNode *node, std::vector<SimpleMDAGNode*> &mdagDataArray, int onePastLastCreatedTransitionSetIndex)
    {
        int pivotIndex = onePastLastCreatedTransitionSetIndex;
        node->setTransitionSetBeginIndex(onePastLastCreatedTransitionSetIndex);
        onePastLastCreatedTransitionSetIndex += node->getOutgoingTransitionCount();
        std::map<char, MDAGNode*> transitionTreeMap = node->getOutgoingTransitions();
        MDAGNode *transitionTargetNode;
        for (auto &pair : transitionTreeMap )
        {
            char transitionLabelChar = (char) pair.first;
            transitionTargetNode = (MDAGNode*) pair.second;
            mdagDataArray[pivotIndex] = new SimpleMDAGNode(transitionLabelChar, transitionTargetNode->isAcceptNode(), transitionTargetNode->getOutgoingTransitionCount());
            if (transitionTargetNode->getTransitionSetBeginIndex() == -1)
                onePastLastCreatedTransitionSetIndex = this->createSimpleMDAGTransitionSet(transitionTargetNode, mdagDataArray, onePastLastCreatedTransitionSetIndex);
            mdagDataArray[pivotIndex++]->setTransitionSetBeginIndex(transitionTargetNode->getTransitionSetBeginIndex());
        }
        /*for (Iterator i$ = transitionTreeMap.entrySet().iterator(); i$.hasNext(); mdagDataArray[pivotIndex++]->setTransitionSetBeginIndex(transitionTargetNode->getTransitionSetBeginIndex()))
        {
            Map.Entry<char, MDAGNode> transitionKeyValuePair = (Map.Entry) i$.next();
            char transitionLabelChar = (char) transitionKeyValuePair.getKey();
            transitionTargetNode = (MDAGNode*) transitionKeyValuePair->getValue();
            mdagDataArray[pivotIndex] = new SimpleMDAGNode(transitionLabelChar, transitionTargetNode.isAcceptNode(), transitionTargetNode.getOutgoingTransitionCount());
            if (transitionTargetNode->getTransitionSetBeginIndex() == -1)
                onePastLastCreatedTransitionSetIndex = this->createSimpleMDAGTransitionSet(transitionTargetNode, mdagDataArray, onePastLastCreatedTransitionSetIndex);
        }*/
        return onePastLastCreatedTransitionSetIndex;
    }
    void MDAG::simplify()
    {
        if (this->sourceNode != nullptr)
        {
            this->mdagDataArray =  std::vector<SimpleMDAGNode*>(this->transitionCount);
            this->createSimpleMDAGTransitionSet(this->sourceNode, this->mdagDataArray, 0);
            this->simplifiedSourceNode = new SimpleMDAGNode('\u0000', false, this->sourceNode->getOutgoingTransitionCount());
            this->sourceNode = nullptr;
            this->equivalenceClassMDAGNodeHashMap.clear();
        }
    }
    bool MDAG::contains(std::string &str)
    {
        if (this->sourceNode != nullptr)
        {
            MDAGNode *targetNode = this->sourceNode->transition(str);
            return targetNode != nullptr && targetNode->isAcceptNode();
        }
        else
        {
            SimpleMDAGNode *targetNode = SimpleMDAGNode::traverseMDAG(this->mdagDataArray, this->simplifiedSourceNode, str);
            return targetNode != nullptr && targetNode->isAcceptNode();
        }
    }
    void MDAG::getStrings(std::unordered_set<std::string> &strHashSet, MDAG::SearchCondition searchCondition, std::string &searchConditionString, std::string &prefixString, std::map<char, MDAGNode*> &transitionTreeMap)
    {
        std::string newPrefixString;
        MDAGNode *currentNode;
        for (auto &pair : transitionTreeMap )
        {
            newPrefixString = prefixString + pair.first;
            currentNode = pair.second;
            if (currentNode->isAcceptNode() && searchCondition::satisfiesCondition(newPrefixString, searchConditionString))
                strHashSet.insert(newPrefixString);
            this->getStrings(strHashSet, searchCondition, searchConditionString, newPrefixString, currentNode->getOutgoingTransitions());
        }
        /*for (Iterator i$ = transitionTreeMap.entrySet().iterator(); i$.hasNext(); this->getStrings(strHashSet, searchCondition, searchConditionString, newPrefixString, currentNode->getOutgoingTransitions()))
        {
            Map.Entry<char, MDAGNode> transitionKeyValuePair = (Map.Entry) i$.next();
            newPrefixString = prefixString + transitionKeyValuePair->getKey();
            currentNode = (MDAGNode*) transitionKeyValuePair->getValue();
            if (currentNode->isAcceptNode() && searchCondition.satisfiesCondition(newPrefixString, searchConditionString))
                strHashSet.insert(newPrefixString);
        }*/
    }
    void MDAG::getStrings(std::unordered_set<std::string> &strHashSet, MDAG::SearchCondition searchCondition, std::string &searchConditionString, std::string &prefixString, SimpleMDAGNode *node)
    {
        int transitionSetBegin = node->getTransitionSetBeginIndex();
        int onePastTransitionSetEnd = transitionSetBegin + node->getOutgoingTransitionSetSize();
        for (int i = transitionSetBegin; i < onePastTransitionSetEnd; ++i)
        {
            SimpleMDAGNode *currentNode = this->mdagDataArray[i];
            std::string newPrefixString = prefixString + currentNode->getLetter();
            if (currentNode->isAcceptNode() && searchCondition.satisfiesCondition(newPrefixString, searchConditionString))
                strHashSet.insert(newPrefixString);
            this->getStrings(strHashSet, searchCondition, searchConditionString, newPrefixString, currentNode);
        }
    }
    std::unordered_set<std::string> MDAG::getAllStrings()
    {
        std::unordered_set<std::string> strHashSet;
        if (this->sourceNode != nullptr)
            this->getStrings(strHashSet, MDAG::SearchCondition::NO_SEARCH_CONDITION, (std::string) nullptr, "",  this->sourceNode->getOutgoingTransitions());
        else
            this->getStrings(strHashSet, MDAG::SearchCondition::NO_SEARCH_CONDITION, (std::string) nullptr, "", (SimpleMDAGNode*) this->simplifiedSourceNode);
        return strHashSet;
    }
    std::unordered_set<std::string> MDAG::getStringsStartingWith(std::string &prefixStr)
    {
        std::unordered_set<std::string> strHashSet;
        if (this->sourceNode != nullptr)
        {
            MDAGNode *originNode = this->sourceNode->transition(prefixStr);
            if (originNode != nullptr)
            {
                if (originNode->isAcceptNode())
                    strHashSet.insert(prefixStr);
                this->getStrings(strHashSet, MDAG::SearchCondition::PREFIX_SEARCH_CONDITION, prefixStr, prefixStr, originNode->getOutgoingTransitions());
            }
        }
        else
        {
            SimpleMDAGNode *originNode = SimpleMDAGNode::traverseMDAG(this->mdagDataArray, this->simplifiedSourceNode, prefixStr);
            if (originNode != nullptr)
            {
                if (originNode->isAcceptNode())
                    strHashSet.insert(prefixStr);
                this->getStrings(strHashSet, MDAG::SearchCondition::PREFIX_SEARCH_CONDITION, prefixStr, prefixStr, originNode);
            }
        }
        return strHashSet;
    }
    std::unordered_set<std::string> MDAG::getStringsWithSubstring(std::string &str)
    {
        std::unordered_set<std::string> strHashSet;
        if (this->sourceNode != nullptr)
            this->getStrings(strHashSet, MDAG::SearchCondition::SUBSTRING_SEARCH_CONDITION, str, "", this->sourceNode->getOutgoingTransitions());
        else
            this->getStrings(strHashSet, MDAG::SearchCondition::SUBSTRING_SEARCH_CONDITION, str, "", this->simplifiedSourceNode);
        return strHashSet;
    }
    std::unordered_set<std::string> MDAG::getStringsEndingWith(std::string &suffixStr)
    {
        std::unordered_set<std::string> strHashSet;
        if (this->sourceNode != nullptr)
            this->getStrings(strHashSet, MDAG::SearchCondition::SUFFIX_SEARCH_CONDITION, suffixStr, "", this->sourceNode->getOutgoingTransitions());
        else
            this->getStrings(strHashSet, MDAG::SearchCondition::SUFFIX_SEARCH_CONDITION, suffixStr, "", this->simplifiedSourceNode);
        return strHashSet;
    }
    Object MDAG::getSourceNode()
    {
        return this->sourceNode != nullptr ? this->sourceNode : this->simplifiedSourceNode;
    }
    std::vector<SimpleMDAGNode*> MDAG::getSimpleMDAGArray()
    {
        return this->mdagDataArray;
    }
    std::set<char> MDAG::getTransitionLabelSet()
    {
        return this->charTreeSet;
    }
    boolean MDAG::isAcceptNode(Object nodeObj)
    {
        if (nodeObj != nullptr)
        {
            Class nodeObjClass = nodeObj.getClass();
            if (nodeObjClass.equals(MDAGNode.class))
                    return ((MDAGNode) nodeObj).isAcceptNode();
            if (nodeObjClass.equals(SimpleMDAGNode.class))
                return ((SimpleMDAGNode) nodeObj).isAcceptNode();
        }
    }
    static enum MDAG::SearchCondition {
        NO_SEARCH_CONDITION,
        PREFIX_SEARCH_CONDITION,
        SUBSTRING_SEARCH_CONDITION,
        SUFFIX_SEARCH_CONDITION;
        private SearchCondition() {}
        public boolean satisfiesCondition(String str1, String str2)
        {
            boolean satisfiesSearchCondition;
            switch (this)
            {
                case PREFIX_SEARCH_CONDITION:
                    satisfiesSearchCondition = str1.startsWith(str2);
                    break;
                case SUBSTRING_SEARCH_CONDITION:
                    satisfiesSearchCondition = str1.contains(str2);
                    break;
                case SUFFIX_SEARCH_CONDITION:
                    satisfiesSearchCondition = str1.endsWith(str2);
                    break;
                default:
                    satisfiesSearchCondition = true;
            }
            return satisfiesSearchCondition;
        }
    }
}