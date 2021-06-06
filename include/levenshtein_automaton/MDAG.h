#pragma once

#include "levenshtein_automaton/MDAGNode.h"
#include "levenshtein_automaton/SimpleMDAGNode.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <tuple>

namespace la
{
    class MDAG{
    private:
        MDAGNode *sourceNode{new MDAGNode{false}};
        SimpleMDAGNode *simplifiedSourceNode;
        std::unordered_map<MDAGNode *, MDAGNode *> equivalenceClassMDAGNodeHashMap;
        std::vector<SimpleMDAGNode *> mdagDataArray;
        std::set<char> charTreeSet;
        int transitionCount;
    public:
        MDAG(const std::vector<std::string> &strCollection);
        void addStrings(const std::vector<std::string> &strCollection); 
    private:
        int calculateMinimizationProcessingStartIndex(const std::string &prevStr, const std::string &currStr);
        std::string determineLongestPrefixInMDAG(const std::string &str);
        std::tuple<int, MDAGNode *> getTransitionPathFirstConfluenceNodeData(MDAGNode *originNode, const std::string &str);
        void replaceOrRegister(MDAGNode *originNode, const std::string &str);
        void addTransitionPath(MDAGNode *originNode, const std::string &str);
        void removeTransitionPathRegisterEntries(const std::string &str);
        void cloneTransitionPath(MDAGNode *pivotConfluenceNode, const std::string &transitionStringToPivotNode, const std::string &str);
        void addStringInternal(const std::string &str);
    };
}