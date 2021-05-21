cmake_minimum_required (VERSION 3.0.0)

project (levenshtein_automaton VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(
		src/levenshtein_automaton/State.cpp
		include/levenshtein_automaton/State.h
		include/levenshtein_automaton/AugBitSet.h
		src/levenshtein_automaton/Position.cpp
		include/levenshtein_automaton/Position.h src/levenshtein_automaton/ParametricState.cpp include/levenshtein_automaton/ParametricState.h
)

set(LA
		src/levenshtein_automaton/SimpleMDAGNode.cpp
		src/levenshtein_automaton/MDAGNode.cpp
		src/levenshtein_automaton/MDAG.cpp )

add_executable (test tests/test.cpp ${LA})