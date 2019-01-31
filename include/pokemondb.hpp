#ifndef _POKEMONDB_HPP_
#define _POKEMONDB_HPP_

#include <sstream>

#include "stats.hpp"

class PokemonDB {
    private:
        static const unsigned int POKEMON_OFFSET = 84;
        static const unsigned int MOVE_OFFSET = 8;
        static const unsigned int ITEM_OFFSET = 3;

        bool loaded;
        std::istringstream db_binary_species;
        std::istringstream db_binary_moves;
        std::istringstream db_binary_items;

        bool load();

    public:
        PokemonDB();
        PokemonDB(const PokemonDB& theDB) = delete;

        bool isLoaded() const { return loaded; }
        uint16_t getPokemonStat(const unsigned int thePokedexNumber, const Stats::Stat& theStat) const;
        std::array<uint8_t, 2> getPokemonType(const unsigned int thePokedexNumber) const;
        unsigned int getMoveBasePower(const unsigned int theMoveIndex) const;
        unsigned int getMoveBasePowerZ(const unsigned int theMoveIndex) const;
        bool isSignatureZ(const unsigned int theMoveIndex) const;
        uint8_t getMoveType(const unsigned int theMoveIndex) const;
        uint8_t getMoveCategory(const unsigned int theMoveIndex) const;
        uint8_t isMoveSpread(const unsigned int theMoveIndex) const;
        std::array<uint16_t, 3> getPokemonAbilities(const unsigned int thePokedexNumber) const;
        bool isItemRemovable(const unsigned int theItemIndex) const;
        bool isItemReducingBerry(const unsigned int theItemIndex) const;
        uint8_t getReducingBerryType(const unsigned int theItemIndex) const;
};

#endif
