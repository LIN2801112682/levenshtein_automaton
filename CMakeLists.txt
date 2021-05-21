cmake_minimum_required (VERSION 3.0.0)

project (levenshtein_automaton VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(LA
		src/levenshtein_automaton/SimpleMDAGNode.cpp
		src/levenshtein_automaton/MDAGNode.cpp
		src/levenshtein_automaton/MDAG.cpp )

add_executable (test tests/test.cpp ${LA})