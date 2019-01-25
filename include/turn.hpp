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

        friend class Pokemon;
        std::vector<std::pair<Pokemon, Move>> moves;
        unsigned int hits;

        unsigned int getHits() const { return hits; }
        Type type;

    public:
        Turn();

        void addMove(const Pokemon& thePokemon, const Move& theMove);
        void setHits(const unsigned int theHits) { hits = theHits; }

        Type getType() const { return type; }
        unsigned int getMoveNum() const { return moves.size(); }
        std::vector<std::pair<Pokemon, Move>> getMoves() const { return moves; }
};

#endif // _TURN_HPP_
