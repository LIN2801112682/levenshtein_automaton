
#ifndef LEVENSHTEIN_AUTOMATON_PARAMETRICSTATE_H
#define LEVENSHTEIN_AUTOMATON_PARAMETRICSTATE_H

#include <vector>

namespace la{
   class ParametricState {
   private:
       std::vector<int> memberPositionBoundaryOffsetArray;
       std::vector<int>  memberPositionEArray;
       std::vector<bool> memberPositionTArray;
       int transitionBoundaryOffset{0};
   public:

   private:

//       const int  *memberPositionBoundaryOffsetArray;
//       const int  *memberPositionEArray;
//       const bool *memberPositionTArray;

   public:
       ParametricState(State *state);
        ParametricState(State *state, int transitionBoundaryOffset);

        int hashCode();

       bool equals(ParametricState *obj);

       State *createActualState(int minimalBoundary);

       int getTransitionBoundaryOffset();

       int getLargestPositionOffset();

       std::string toString();

       int memberPositionCount;
   };
}
#endif //LEVENSHTEIN_AUTOMATON_PARAMETRICSTATE_H
