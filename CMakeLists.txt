cmake_minimum_required (VERSION 3.0.0)

project (levenshtein_automaton VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(
		src/State.cpp
		src/Position.cpp
		src/ParametricState.cpp
		src/LevenshteinAutomaton.cpp
		src/MDAG.cpp
		src/MDAGNode.cpp
		src/SimpleMDAGNode.cpp
		src/types.cpp
)


add_executable (test tests/test.cpp ${LA})