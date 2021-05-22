#include "../include/Position.h"


//namespace std
//{
//    template<>
//    struct less<la::Position>
//    {
//        bool
//        operator()(la::Position& __x, la::Position& __y) const
//        { return __x.compareTo(__y) == -1; }
//    };
//}

namespace la
{
    ElementaryTransitionTerm::ElementaryTransitionTerm(int iOffset, int eOffset, bool representsTransposition) {
        I_OFFSET = iOffset;
        E_OFFSET = eOffset;
        REPRESENTS_PRETRANSPOSITION = representsTransposition;
    }
    int ElementaryTransitionTerm::getI_OFFSET(){
        return I_OFFSET;
    }
    int ElementaryTransitionTerm::getE_OFFSET() {
        return E_OFFSET;
    }
    bool ElementaryTransitionTerm::getREPRESENTS_PRETRANSPOSITION() {
        return REPRESENTS_PRETRANSPOSITION;
    }
    Position* execute(Position *p,ElementaryTransitionTerm *elementaryTransitionTerm, int hitIndex)
    {
        if (elementaryTransitionTerm != &FAILURE) {
            int newI = p->getI() + (elementaryTransitionTerm == &DELETION ? hitIndex + 1 : elementaryTransitionTerm->getI_OFFSET());        //for deletion, newI represents the # of boundaries to hitIndex (0-based arrays, so we require +1)
            int newE = p->getE() + (elementaryTransitionTerm == &DELETION ? (hitIndex + 1) - 1 : elementaryTransitionTerm->getE_OFFSET());  //for deletion, newE represents the # of boundaries up to but not including hitIndex (0-based arrays, so simply hitIndex)
//            内存泄漏
            return new Position(newI, newE, elementaryTransitionTerm->getREPRESENTS_PRETRANSPOSITION());
        } else return NULL;
    }
    Position::Position(int I, int E, bool T){
        this->I = I;
        this->E = E;
        this->T = T;
    }

    int Position::getI(){
        return I;
    }

    int Position::getE(){
        return E;
    }

    bool Position::getT(){
        return T;
    }


    ElementaryTransitionTerm* Position::procureTransition(EditDistanceRelationType &edRelationType, StateRelevantSubwordSizeType &sRSSizeType, PositionType &pType, RelevantSubwordHitIndexType &rsHitIndexType){
        switch(edRelationType)
        {
            case AT_ZERO_AND_NOT_AT_MAX:            //we are allowed to execute edit operations (none have been conducted yet)
            {
                switch(sRSSizeType)
                {
                    case ATLEAST_TWO:               //if there are at least one boundary in the automaton's String after this position's boundary
                    {
                        switch(rsHitIndexType)
                        {
                            case FIRST_INDEX:       return MATCH_TRANSITION;
                            case SECOND_INDEX:      return INSERTION_PRETRANSPOSITION_SUBSTITUTION_DELETION_TRANSITION;
                            case TRAILING_INDEX:    return INSERTION_SUBSTITUTION_DELETION_TRANSITION;
                            default:                return INSERTION_SUBSTITUTION_TRANSITION;
                        }
                    }
                    case ONE:                       //if this position's boundary is the last boundary in the automaton's String
                    {
                        switch(rsHitIndexType)
                        {
                            case FIRST_INDEX:       return MATCH_TRANSITION;
                            default:                return INSERTION_SUBSTITUTION_TRANSITION;
                        }
                    }
                    default:                        return INSERTION_TRANSITION;    //there are no more boundaries in the automaton's String
                }
            }
            case NOT_AT_ZERO_AND_NOT_AT_MAX:        //we are allowed to execute more edit operations (some have already been conducted)
            {
                switch(sRSSizeType)
                {
                    case ATLEAST_TWO:               //if there are at least one boundary in the automaton's String after this position's boundary
                    {
                        switch(pType)
                        {
                            case STANDARD_POSITION:
                            {
                                switch(rsHitIndexType)
                                {
                                    case FIRST_INDEX:       return MATCH_TRANSITION;
                                    case SECOND_INDEX:      return INSERTION_PRETRANSPOSITION_SUBSTITUTION_DELETION_TRANSITION;
                                    case TRAILING_INDEX:    return INSERTION_SUBSTITUTION_DELETION_TRANSITION;
                                    default:                return INSERTION_SUBSTITUTION_TRANSITION;
                                }
                            }
                            default:                //transposition position
                            {
                                switch(rsHitIndexType)
                                {
                                    case FIRST_INDEX:       return TRANSPOSITION_TRANSITION;
                                    default:                return FAILURE_TRANSITION;
                                }
                            }
                        }
                    }
                    case ONE:                       //if this position's boundary is the last boundary in the automaton's String
                    {
                        switch(rsHitIndexType)
                        {
                            case FIRST_INDEX:       return MATCH_TRANSITION;
                            default:                return INSERTION_SUBSTITUTION_TRANSITION;
                        }
                    }
                    default:                        return INSERTION_TRANSITION;     //there are no more boundaries in the automaton's String
                }
            }
            default:                //we are NOT allowed to execute more edit operations
            {
                switch(sRSSizeType)
                {
                    case ZERO:                      return FAILURE_TRANSITION;   //there are no more boundaries in the automaton's String
                    default:                        //there is at least one boundary left in this automaton's String
                    {
                        switch(pType)               //we use the position's type instead of its relevant subword size to dictate processing from here because doing so produces
                        {                           //mutually exclusive, covering cases and results in code that is simpler and mirrors the paper's transition table
                            case STANDARD_POSITION:
                            {
                                switch(rsHitIndexType)
                                {
                                    case FIRST_INDEX:       return MATCH_TRANSITION;
                                    default:                return FAILURE_TRANSITION;
                                }
                            }
                            default:                //transposition position
                            {
                                switch(rsHitIndexType)
                                {
                                    case FIRST_INDEX:       return TRANSPOSITION_TRANSITION;
                                    default:                return FAILURE_TRANSITION;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    la::State* Position::transitionInternal(int maxEditDistance, int relevantSubwordSize, int hitIndex){
        //Determine the EditDistanceRelationType representing the relationship between E and maxEditDistance
        EditDistanceRelationType edRelationType = (E < maxEditDistance ? (E == 0 ? AT_ZERO_AND_NOT_AT_MAX
                                                                                 : NOT_AT_ZERO_AND_NOT_AT_MAX) : AT_MAX);

        //Determine the StateRelevantSubwordSizeType representing the size of this Position's relevant subword
        StateRelevantSubwordSizeType stateRSSizeType = (relevantSubwordSize >= 2 ? ATLEAST_TWO
                                                                                 : (relevantSubwordSize == 1 ? ONE : ZERO));

        //Determine the PositionType representing this Position's type
        PositionType pType = (T ? TRANSPOSITION_POSITION : STANDARD_POSITION);

        //Determine the RelevantSubwordHitIndexType representing the category of location index-types hitIndex falls into
        RelevantSubwordHitIndexType rsHitIndexType;
        switch(hitIndex)
        {
            case -1:    rsHitIndexType = NO_INDEX;          break;
            case 0:     rsHitIndexType = FIRST_INDEX;       break;
            case 1:     rsHitIndexType = SECOND_INDEX;      break;
            default:    rsHitIndexType = TRAILING_INDEX;    break;
        }
        ElementaryTransitionTerm* elementaryTransition = procureTransition(edRelationType, stateRSSizeType, pType, rsHitIndexType);
        //内存泄漏
        std::vector<Position> possibleNewPositionHashSet;
        int  num = 1;
        for(int i = 0 ; i < num ; i++)
        {
            ElementaryTransitionTerm *currentTerm = &elementaryTransition[num];
            Position* transitionPosition = currentTerm->execute(this, currentTerm, hitIndex);
            if(transitionPosition != NULL) possibleNewPositionHashSet.push_back(*transitionPosition);
        }
        /////

        if(!possibleNewPositionHashSet.empty())       //If the transition execution resulted in at least one valid Position, produce an
        {                                                   //array version of the result set, sort it and use it to create and return a State
            sort( possibleNewPositionHashSet.begin(), possibleNewPositionHashSet.end());
            possibleNewPositionHashSet.erase(unique(possibleNewPositionHashSet.begin(), possibleNewPositionHashSet.end()), possibleNewPositionHashSet.end());
//            new
            State* stateptr = new State(possibleNewPositionHashSet);
            return stateptr;
        }
        else
            return nullptr;
    }

    int * Position::procurePositionTransitionData(int maxEditDistance, int relevantSubwordStartIndex, la::AugBitSet &parentStateRelevantSubwordCharacteristicVector) {
        //int which will contain the value of this Position's relevant subword size
        int relevantSubwordSize;

        //int which will denote the index of the first occurance of the char
        //that the transition is being carried out for in this Position's relevant subword
        int firstHitIndex;

        //AugBitSet which will represent the characteristic vector of this Position's relevant subowrd
        la::AugBitSet relevantSubwordCharacteristicVector;

        if(relevantSubwordStartIndex < parentStateRelevantSubwordCharacteristicVector.getRelevantBitSetSize())
        {
            relevantSubwordSize = min(maxEditDistance - E + 1, parentStateRelevantSubwordCharacteristicVector.getRelevantBitSetSize() - relevantSubwordStartIndex);

            relevantSubwordCharacteristicVector = parentStateRelevantSubwordCharacteristicVector.get(relevantSubwordStartIndex,
                                                                                                     relevantSubwordStartIndex + relevantSubwordSize);
            firstHitIndex = relevantSubwordCharacteristicVector.nextSetBit(0);
        }
        else
        {
            relevantSubwordSize = 0;
            firstHitIndex = -1;
        }

        return new int[]{relevantSubwordSize, firstHitIndex};
    }
    la::State* Position::transition(int maxEditDistance, int parentStateRelevantSubwordLocationIndex, la::AugBitSet &parentStateRelevantSubwordCharacteristicVector) {
        int* positionTransitionDataArray = procurePositionTransitionData(maxEditDistance, parentStateRelevantSubwordLocationIndex, parentStateRelevantSubwordCharacteristicVector);
        return transitionInternal(maxEditDistance, positionTransitionDataArray[0], positionTransitionDataArray[1]);
    }

    bool Position::subsumes(Position &p, int maxEditDistance) {
        if(this->T)
        {
            if(p.T)   return (this->E < p.E && this->I == p.I);
            else      return ((p.E == maxEditDistance) && (p.E > this->E) && (this->I == p.I));
        }
        else
        {
            if(p.T)   return (this->E < p.E && !(abs(p.I - (this->I - 1)) > (p.E - this->E)));
            else      return (this->E < p.E && !(abs(p.I - this->I) > (p.E - this->E)));
        }
    }

    int Position::boolcompare(bool b1, bool b2) {
        if(b1 == true){
            if(b2 == false)return 1;
            else return 0;
        }else{
            if(b2 == false)return 0;
            else return -1;
        }
    }

    int Position::compareTo(Position &p2) {
        if(this->I == p2.I)
        {
            int eComparisonResultInt = this->E - p2.E;
            return (eComparisonResultInt == 0 ? boolcompare(this->T, p2.T) : eComparisonResultInt);
        }
        else if(this->I < p2.I)
            return -1;
        else
            return 1;
    }

}