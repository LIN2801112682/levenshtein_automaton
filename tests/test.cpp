#include <MDAG.h>
#include <LevenshteinAutomaton.h>
#include <iostream>
#include <cstring>
#include "../include/State.h"
#include "../include/Position.h"
namespace test
{
    using LevenshteinAutomaton = la::LevenshteinAutomaton;
    using MDAG = la::MDAG;

    void main(){
        std::ifstream infile("resources/wordList.txt");
        // 根据字典构建
        std::string lineStr;
        char* myArrayList;
        if(infile){
          while(getline(infile,lineStr)){
               lineStr.append(lineStr);
          }
          strcpy(myArrayList,lineStr.c_str());
      }
        MDAG myMDAG = MDAG(myArrayList);
        // 读取测试文件搜索测试
        std::ifstream reader("resources/wordsToTest.txt");
        std::string testString;
        int maxEdit = 2;
        std::vector<int> method_3_time;

        while (getline(reader,testString))
        {
            std::cout << "======================================================" << std::endl;
            std::cout << "search token: " << testString << ";maxEdit= " << maxEdit << std::endl;
            std::list<std::string> resultList3 = LevenshteinAutomaton::iterativeFuzzySearch(maxEdit, testString, myMDAG);
            std::cout << "Method3 iterativeFuzzySearch：" << std::endl;
            std::cout << "matched token size: " << resultList3.size() << std::endl;
        }
        std::cout << "===================================" << std::endl;
        reader.close();
    }
}
