#pragma once

#include <string>
#include <unordered_map>
#include <list>
#include <unordered_set>

namespace la
{
    class ParametricState;
    class AugBitSet;
    class State;
    class Position;
    class MDAG;

    class LevenshteinAutomaton
    {
    private:
        static const std::unordered_map<int, std::unordered_map<ParametricState, std::unordered_map<AugBitSet, ParametricState>>> transitionHashMapContainerHashMap;
        static const State initialState;
        LevenshteinAutomaton();
    public:
        static int computeEditDistance(std::string &str1, std::string &str2);
        static bool isWithinEditDistanceNonAutomaton(int maxEditDistance, std::string &str1, std::string &str2);
        static bool isWithinEditDistance(int maxEditDistance, std::string &str1, std::string &str2);
    private:
        static void procureNewBasedStates(std::list<State> &basedStateLinkedList, Position &p, int maxEditDistance);
        static std::list<State> procureBasedStates(Position &basePosition, int maxRelevantSubwordSize, int maxEditDistance);
        static std::unordered_set<ParametricState> procureParametricStates(int maxRelevantSubwordSize, int maxEditDistance);
        static std::vector<AugBitSet> produceBinaryPermutations(int maxBitCount);
        static void createParametricStateTransitionMap(int maxEditDistance);
        static std::vector<void *> createProcessingStepStackEntry(std::string &transitionPathString, void *mdagTransitionPathEndNode, State &lAutomatonTransitionPathEndState);
        static std::vector<void *> createProcessingStepStackEntry(std::string &transitionPathString, void *mdagTransitionPathEndNode,
                                                       State &lAutomatonTransitionPathEndState, ParametricState &lAutomatonTransitionPathEndParametricState);
    public:
        static std::list<std::string> iterativeFuzzySearch(int maxEditDistance, std::string &automatonString, MDAG &mdag);
        static std::list<std::string> tableFuzzySearch(int maxEditDistance, std::string &automatonString, MDAG &mdag);
        static std::list<std::string> fuzzySearchNonAutomaton(int maxEditDistance, std::string &pertinentStr, std::vector<std::string> &strCollection);
    private:
        static bool isAcceptingPosition(Position &p, int automatonStringLength, int automatonMaxEditDistance);
    public:
        static bool isAcceptState(State &s, int automatonStringLength, int automatonMaxEditDistance);
        static void printTransitionMap(int maxEditDistance);
    };
}