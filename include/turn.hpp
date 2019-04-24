#ifndef _TURN_HPP_
#define _TURN_HPP_

#include <vector>

#include "pokemon.hpp"
#include "move.hpp"

class Pokemon;

class Turn {
    private:
        enum Type {
            TYPE_NONE,
            MIXED,
            PHYSICAL,
            SPECIAL
        };

        std::vector<std::pair<Pokemon, Move>> getMovesEffective() const;
        bool isFoulPlay() const { return is_foul_play; } //this is needed because foul play is a special move, and we have to deactivate the PRIORITIZE DEFENSE OPTION if any of the turn contains the foul play move

        friend class Pokemon;
        std::vector<std::pair<Pokemon, Move>> moves;
        unsigned int hits;
        bool is_foul_play;

        Type type;

    public:
        Turn();

        void addMove(const Pokemon& thePokemon, const Move& theMove);
        void setHits(const unsigned int theHits) { hits = theHits; }

        Type getType() const { return type; }
        unsigned int getHits() const { return hits; }
        unsigned int getMoveNum() const { return moves.size(); }
        std::vector<std::pair<Pokemon, Move>> getMoves() const { return moves; }
        unsigned int getMovesEffectiveNumber() const { return getMovesEffective().size(); }
};

#endif // _TURN_HPP_
