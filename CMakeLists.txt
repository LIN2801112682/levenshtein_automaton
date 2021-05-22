cmake_minimum_required (VERSION 3.0.0)

project (levenshtein_automaton VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
aux_source_directory(src)
set(
		src/State.cpp
		src/Position.cpp
		src/ParametricState.cpp
)

set(LA
		src/SimpleMDAGNode.cpp
		src/MDAGNode.cpp
		src/MDAG.cpp)

add_executable (test tests/test.cpp ${LA})