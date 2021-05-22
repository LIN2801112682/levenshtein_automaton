#pragma once
#include <string>
#include <cmath>
#include <algorithm>
//#include <set>
//#include <list>
#include <vector>

namespace la {
    namespace la { class State; }
    namespace la { class AugBitSet; }
    class Position;
    class ElementaryTransitionTerm;

    static enum EditDistanceRelationType {
        AT_MAX,
        NOT_AT_ZERO_AND_NOT_AT_MAX,
        AT_ZERO_AND_NOT_AT_MAX
    };
    static enum StateRelevantSubwordSizeType {
        ATLEAST_TWO,
        ONE,
        ZERO
    };
    static enum PositionType {
        STANDARD_POSITION,
        TRANSPOSITION_POSITION
    };
    static enum RelevantSubwordHitIndexType {
        FIRST_INDEX,
        SECOND_INDEX,
        TRAILING_INDEX,
        NO_INDEX
    };

    class ElementaryTransitionTerm{
    private:
        int I_OFFSET;
        int E_OFFSET;
        bool REPRESENTS_PRETRANSPOSITION;
    public:
        int getI_OFFSET();
        int getE_OFFSET();
        bool getREPRESENTS_PRETRANSPOSITION();
        ElementaryTransitionTerm(int iOffset, int eOffset, bool representsTransposition);
        Position* execute(Position *p,ElementaryTransitionTerm *elementaryTransitionTerm, int hitIndex);


    };
    static ElementaryTransitionTerm MATCH = ElementaryTransitionTerm(1,0,false);
    static ElementaryTransitionTerm INSERTION = ElementaryTransitionTerm(0,1,false);
    static ElementaryTransitionTerm PRETRANSPOSITION = ElementaryTransitionTerm(0,1,true);
    static ElementaryTransitionTerm TRANSPOSITION = ElementaryTransitionTerm(2,0,false);
    static ElementaryTransitionTerm SUBSTITUTION = ElementaryTransitionTerm(1,1,false);
    static ElementaryTransitionTerm DELETION = ElementaryTransitionTerm(0,0,false);
    static ElementaryTransitionTerm FAILURE = ElementaryTransitionTerm(0,0,false);

    static ElementaryTransitionTerm MATCH_TRANSITION[] = {MATCH};
    static ElementaryTransitionTerm INSERTION_PRETRANSPOSITION_SUBSTITUTION_DELETION_TRANSITION[] = {INSERTION,PRETRANSPOSITION,SUBSTITUTION,DELETION};
    static ElementaryTransitionTerm INSERTION_SUBSTITUTION_DELETION_TRANSITION[] = {INSERTION,SUBSTITUTION,DELETION};
    static ElementaryTransitionTerm INSERTION_SUBSTITUTION_TRANSITION[] = {INSERTION,SUBSTITUTION};
    static ElementaryTransitionTerm TRANSPOSITION_TRANSITION[] = {TRANSPOSITION};
    static ElementaryTransitionTerm INSERTION_TRANSITION[] = {INSERTION};
    static ElementaryTransitionTerm FAILURE_TRANSITION[] = {FAILURE};


    class Position{
    private:
        int I;
        int E;
        bool T;
        EditDistanceRelationType edRelationType;
        StateRelevantSubwordSizeType sRSSizeType;
        PositionType pType;
        RelevantSubwordHitIndexType rsHitIndexType;
        ElementaryTransitionTerm* procureTransition(EditDistanceRelationType &edRelationType, StateRelevantSubwordSizeType &sRSSizeType, PositionType &pType, RelevantSubwordHitIndexType &rsHitIndexType);
        la::State* transitionInternal(int maxEditDistance, int relevantSubwordSize, int hitIndex);
        int boolcompare(bool b1,bool b2);


    public:
        Position(int I, int E, bool T);
        int getI();
        int getE();
        bool getT();
        int* procurePositionTransitionData(int maxEditDistance, int relevantSubwordStartIndex, la::AugBitSet &parentStateRelevantSubwordCharacteristicVector);
        la::State* transition(int maxEditDistance, int parentStateRelevantSubwordLocationIndex, la::AugBitSet &parentStateRelevantSubwordCharacteristicVector);
        bool subsumes(Position &p, int maxEditDistance);
        int compareTo(Position &p2);

    };
}