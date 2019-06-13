#ifndef _POKEMONDB_HPP_
#define _POKEMONDB_HPP_

/*This class manages the database which contains informations about all the pokemons, moves and items used in this software
this does get automatically loaded whenever constructed, you should always check the isLoaded function to see if the loading
process has been completed correctly
NOTE: This is a Qt-dependant class*/ 

#include <sstream>
#include <QFile>

#include "stats.hpp"
#include "types.hpp"
#include "abilities.hpp"
#include "move.hpp"

class PokemonDB {
    private:
        static const unsigned int POKEMON_OFFSET = 84; //this is the offset between each pokemon in the personal_species file (to be changed if the file format changes)
        static const unsigned int MOVE_OFFSET = 8; //this is the offset between each move in the personal_moves file (to be changed if the file format changes)
        static const unsigned int ITEM_OFFSET = 6; //this is the offset between each pokemon in the personal_items file (to be changed if the file format changes)

        //this constant are used to parse the database file
        static const unsigned int STAT_OFFSET = 0;
        static const unsigned int STAT_SIZE = 6;
        static const unsigned int TYPE_OFFSET = 6;
        static const unsigned int TYPE_SIZE = 2;
        static const unsigned int ABILITY_OFFSET = 24;
        static const unsigned int ABILITY_SIZE = 3;
        static const unsigned int FORM_OFFSET = 28;
        static const unsigned int FORM_SIZE = 2;
        static const unsigned int FORMNUMBER_OFFSET = 32;
        static const unsigned int FORMNUMBER_SIZE = 1;

        static const unsigned int MOVEBP_OFFSET = 0;
        static const unsigned int MOVEBP_SIZE = 2;
        static const unsigned int MOVETYPE_OFFSET = 2;
        static const unsigned int MOVETYPE_SIZE = 1;
        static const unsigned int MOVECATEGORY_OFFSET = 3;
        static const unsigned int MOVECATEGORY_SIZE = 1;
        static const unsigned int MOVESPREAD_OFFSET = 4;
        static const unsigned int MOVESPREAD_SIZE = 1;
        static const unsigned int MOVEBPZ_OFFSET = 5;
        static const unsigned int MOVEBPZ_SIZE = 2;
        static const unsigned int MOVESIGNATUREZ_OFFSET = 7;
        static const unsigned int MOVESIGNATUREZ_SIZE = 1;

        static const unsigned int ITEMREMOVABLE_OFFSET = 0;
        static const unsigned int ITEMREMOVABLE_SIZE = 1;
        static const unsigned int ITEMREDUCINGBERRY_OFFSET = 1;
        static const unsigned int ITEMREDUCINGBERRY_SIZE = 1;
        static const unsigned int ITEMREDUCINGBERRYTYPE_OFFSET = 2;
        static const unsigned int ITEMREDUCINGBERRYTYPE_SIZE = 1;
        static const unsigned int ITEMRESTORINGBERRY_OFFSET = 3;
        static const unsigned int ITEMRESTORINGBERRY_SIZE = 1;
        static const unsigned int ITEMRESTORINGBERRYACTIVATION_OFFSET = 4;
        static const unsigned int ITEMRESTORINGBERRYACTIVATION_SIZE = 1;
        static const unsigned int ITEMRESTORINGBERRYPERCENTAGE_OFFSET = 5;
        static const unsigned int ITEMRESTORINGBERRYPERCENTAGE_SIZE = 1;
        
        bool loaded; //this variable contains the result of the loading process
        
        //this variables contain the binary extracted from the external files
        QByteArray species_data;
        QByteArray moves_data;
        QByteArray items_data;

        unsigned int getPokemonOffset(const unsigned int thePokedexNumber, const unsigned int theForm) const; //this one gets the pokemon's data position into the external database binary file
        unsigned int getMoveOffset(const unsigned int theMoveIndex) const { return MOVE_OFFSET * theMoveIndex; }
        unsigned int getItemOffset(const unsigned int theItemIndex) const { return ITEM_OFFSET * theItemIndex; }

        std::vector<uint8_t> readMovesData(const unsigned int theOffset, const unsigned int theSize) const { return readData(moves_data, theOffset, theSize); }
        std::vector<uint8_t> readItemsData(const unsigned int theOffset, const unsigned int theSize) const { return readData(items_data, theOffset, theSize); }
        std::vector<uint8_t> readSpeciesData(const unsigned int theOffset, const unsigned int theSize) const { return readData(species_data, theOffset, theSize); }
        std::vector<uint8_t> readData(const QByteArray& theData, const unsigned int theOffset, const unsigned int theSize) const; //reads data from the raw database at the specified offset

    public: PokemonDB();
        //we don't really want to copy this class
        PokemonDB(const PokemonDB& theDB) = delete; 
        PokemonDB& operator=(const PokemonDB& theDB) = delete;
        
        bool load();
        bool isLoaded() const { return loaded; }
        
        unsigned int getPokemonStat(const unsigned int thePokedexNumber, const unsigned int theForm, const Stats::Stat& theStat) const { return readSpeciesData(getPokemonOffset(thePokedexNumber, theForm) + STAT_OFFSET, STAT_SIZE)[theStat]; }
        std::array<Type, 2> getPokemonType(const unsigned int thePokedexNumber, const unsigned int theForm) const;
        unsigned int getPokemonFormesNumber(const unsigned int thePokedexNumber) const { return readSpeciesData(getPokemonOffset(thePokedexNumber, 0) + FORMNUMBER_OFFSET, FORMNUMBER_SIZE)[0]; }
        std::array<Ability, 3> getPokemonAbilities(const unsigned int thePokedexNumber, const unsigned int theForm) const;

        unsigned int getMoveBasePower(const unsigned int theMoveIndex) const;
        unsigned int getMoveBasePowerZ(const unsigned int theMoveIndex) const;
        bool isMoveSignatureZ(const unsigned int theMoveIndex) const { return readMovesData(getMoveOffset(theMoveIndex) + MOVESIGNATUREZ_OFFSET, MOVESIGNATUREZ_SIZE)[0]; }
        Type getMoveType(const unsigned int theMoveIndex) const { return static_cast<Type>(readMovesData(getMoveOffset(theMoveIndex) + MOVETYPE_OFFSET, MOVETYPE_SIZE)[0]); }
        Move::Category getMoveCategory(const unsigned int theMoveIndex) const { return static_cast<Move::Category>(readMovesData(getMoveOffset(theMoveIndex) + MOVECATEGORY_OFFSET, MOVECATEGORY_SIZE)[0]); }
        bool isMoveSpread(const unsigned int theMoveIndex) const { return readMovesData(getMoveOffset(theMoveIndex) + MOVESPREAD_OFFSET, MOVESPREAD_SIZE)[0]; }

        bool isItemRemovable(const unsigned int theItemIndex) const { return readMovesData(getItemOffset(theItemIndex) + ITEMREMOVABLE_OFFSET, ITEMREMOVABLE_SIZE)[0]; }
        bool isItemReducingBerry(const unsigned int theItemIndex) const { return readItemsData(getItemOffset(theItemIndex) + ITEMREDUCINGBERRY_OFFSET, ITEMREDUCINGBERRY_SIZE)[0]; }
        bool isItemHPRestoringBerry(const unsigned int theItemIndex) const { return readItemsData(getItemOffset(theItemIndex) + ITEMRESTORINGBERRY_OFFSET, ITEMRESTORINGBERRY_SIZE)[0]; }
        Type getItemReducingBerryType(const unsigned int theItemIndex) const { return static_cast<Type>(readItemsData(getItemOffset(theItemIndex) + ITEMREDUCINGBERRYTYPE_OFFSET, ITEMREDUCINGBERRYTYPE_SIZE)[0]); }
        unsigned int getItemHPRestoringBerryActivation(const unsigned int theItemIndex) const { return readItemsData(getItemOffset(theItemIndex) + ITEMRESTORINGBERRYACTIVATION_OFFSET, ITEMRESTORINGBERRYACTIVATION_SIZE)[0]; }
        unsigned int getItemHPRestoringBerryPercentage(const unsigned int theItemIndex) const { return readItemsData(getItemOffset(theItemIndex) + ITEMRESTORINGBERRYPERCENTAGE_OFFSET, ITEMRESTORINGBERRYPERCENTAGE_SIZE)[0]; }
};

#endif
