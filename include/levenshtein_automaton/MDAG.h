#pragma once

#include <rpcndr.h>
#include <string>
#include <set>
#include <fstream>
#include <unordered_set>
#include "MDAGNode.h"
#include "SimpleMDAGNode.h"

namespace la
{
    class MDAG{
    private:
        MDAGNode *sourceNode = new MDAGNode(false);
        SimpleMDAGNode *simplifiedSourceNode;
        std::unordered_map<MDAGNode*, MDAGNode*> equivalenceClassMDAGNodeHashMap;
        std::vector<SimpleMDAGNode*> mdagDataArray;
        std::set<char> charTreeSet;
        int transitionCount = 0;
        void splitTransitionPath(MDAGNode *originNode, std::string &storedStringSubstr);
        int calculateSoleTransitionPathLength(std::string &str);
        int calculateMinimizationProcessingStartIndex(std::string &prevStr, std::string &currStr);
        std::string determineLongestPrefixInMDAG(std::string &str);
        std::unordered_map<std::string, void*> getTransitionPathFirstConfluenceNodeData(MDAGNode *originNode, std::string &str);
        void replaceOrRegister(MDAGNode *originNode, std::string &str);
        void addTransitionPath(MDAGNode *originNode, std::string &str);
        void removeTransitionPathRegisterEntries(std::string &str);
        void cloneTransitionPath(MDAGNode *pivotConfluenceNode, std::string &transitionStringToPivotNode, std::string &str);
        void addStringInternal(std::string &str);
        int createSimpleMDAGTransitionSet(MDAGNode *node, std::vector<SimpleMDAGNode*> &mdagDataArray, int onePastLastCreatedTransitionSetIndex);
        void getStrings(std::unordered_set<std::string> &strHashSet, MDAG::SearchCondition searchCondition, std::string &searchConditionString, std::string &prefixString, std::map<char, MDAGNode*> &transitionTreeMap);
        void getStrings(std::unordered_set<std::string> &strHashSet, MDAG::SearchCondition searchCondition, std::string &searchConditionString, std::string &prefixString, SimpleMDAGNode *node);
        static enum SearchCondition {
            NO_SEARCH_CONDITION,
            PREFIX_SEARCH_CONDITION,
            SUBSTRING_SEARCH_CONDITION,
            SUFFIX_SEARCH_CONDITION,
            private SearchCondition();
            public boolean satisfiesCondition(String str1, String str2);
            };

    public:
        MDAG(char *filename);
        MDAG(std::vector<std::string> *strCollection);
        void addStrings(std::vector<std::string> *strCollection);
        void addString(std::string &str);
        void removeString(std::string &str);
        void simplify();
        bool contains(std::string &str);
        std::unordered_set<std::string> getAllStrings();
        std::unordered_set<std::string> getStringsStartingWith(std::string &prefixStr);
        std::unordered_set<std::string> getStringsWithSubstring(std::string &str);
        std::unordered_set<std::string> getStringsEndingWith(std::string &suffixStr);
        Object getSourceNode();//
        std::vector<SimpleMDAGNode*> getSimpleMDAGArray();
        std::set<char> getTransitionLabelSet();
        static boolean isAcceptNode(Object nodeObj);
    };
}