#include <string>
#include "./State.h"
#include "./AugBitSet.h"
#include "State.h"

#ifndef LEVENSHTEIN_AUTOMATON_POSITION_H
#define LEVENSHTEIN_AUTOMATON_POSITION_H

namespace la{
    class State;

    class Position{
    public:
        Position() {

        }

        Position(Position *const pPosition) {

        }

        Position(int i, int i1, bool b) {

        }

        bool subsumes(Position position, int &i) {
            return false;
        }

        void remove() {

        }

        int getI() {
            return 0;
        }

        std::string toString() {
            return std::__cxx11::string();
        }
        State *transition(int &i, int &i1, AugBitSet &pSet) {
            return nullptr;
        }

        const int getE() {
            return 0;
        }

        const bool getT() {
            return 0;
        }
    };
}


#endif
