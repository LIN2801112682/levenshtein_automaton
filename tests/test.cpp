#include "common/common.h"
#include "levenshtein_automaton/MDAG.h"
#include <chrono>
#include <iostream>
#include <list>

int main()
{
    auto myArrayList{ readAllLines("../resources/wordList1.txt") };
    auto startTime = std::chrono::high_resolution_clock::now();
    la::MDAG myMDAG{myArrayList};
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Build Success! Cost:" 
        << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() 
        << " ms" << std::endl;

    auto reader{ readAllLines("../resources/wordToTest1.txt") };
    int maxEdit{2};

    for (const auto &testString : reader)
    {
        std::cout << "====" << std::endl;
        std::cout << "search token: " << testString
            << ";maxEdit= " << maxEdit << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        // todo
        //std::list<std::string> resultList{LevenshteinAutomaton.iterativeFuzzySearch(maxEdit, testString, myMDAG)};
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "iterativeFuzzySearch: " << std::endl;
        // todo
        //std::cout << "matched token size: " << resultList.size() << std::endl;
        //std::cout << resultList << std::endl;
        std::cout << "cost time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
            << " ms" << std::endl;
    }
    std::cout << "====" << std::endl;
    return 0;
}