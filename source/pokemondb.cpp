#include "pokemondb.hpp"

#include <QFile>
#include <QDebug>

PokemonDB::PokemonDB() {
    loaded = false;

    loaded = load();
}

bool PokemonDB::load() {
    if( !loaded ) {
        Q_INIT_RESOURCE(resources);

        //loading the species binary (and then converting it to ostringstream because this was pre-qt legacy code)
        QFile input_file_binary_species(":/db/personal_species.bin");
        input_file_binary_species.open(QIODevice::ReadOnly);
        QByteArray species_data = input_file_binary_species.readAll();

        db_binary_species.str(species_data.toStdString());

        //loading the moves binary (and then converting it to ostringstream because this was pre-qt legacy code)
        QFile input_file_binary_moves(":/db/personal_moves.bin"); //HARDCODED BECAUSE I'M LAZY
        input_file_binary_moves.open(QIODevice::ReadOnly);
        QByteArray moves_data = input_file_binary_moves.readAll();

        db_binary_moves.str(moves_data.toStdString());

        //loading the items binary (and then converting it to ostringstream because this was pre-qt legacy code)
        QFile input_file_binary_items(":/db/personal_items.bin"); //HARDCODED BECAUSE I'M LAZY
        input_file_binary_items.open(QIODevice::ReadOnly);
        QByteArray items_data = input_file_binary_items.readAll();

        db_binary_items.str(items_data.toStdString());

        return true;
    }

    else return true;
}

uint16_t PokemonDB::getPokemonStat(const unsigned int thePokedexNumber, const Stats::Stat& theStat) const {
    if( loaded ) {
        unsigned int stat_offset;
        if( theStat == Stats::HP ) stat_offset = 0;
        else if( theStat == Stats::ATK ) stat_offset = 1;
        else if( theStat == Stats::DEF ) stat_offset = 2;
        else if( theStat == Stats::SPE ) stat_offset = 3;
        else if( theStat == Stats::SPATK ) stat_offset = 4;
        else if( theStat == Stats::SPDEF ) stat_offset = 5;
        else stat_offset = 0;

        unsigned int pokedex_offset = POKEMON_OFFSET * thePokedexNumber;

        unsigned int final_offset = pokedex_offset + stat_offset;

        return db_binary_species.str().at(final_offset);
    }

    else return 0;
}

std::array<uint8_t, 2> PokemonDB::getPokemonType(const unsigned int thePokedexNumber) const {
    if( loaded ) {
        unsigned int offset = (POKEMON_OFFSET * thePokedexNumber) + 6;

        std::array<uint8_t, 2> buffer;
        buffer[0] = db_binary_species.str().at(offset);
        buffer[1] = db_binary_species.str().at(offset+1);

        return buffer;
    }

    else return std::array<uint8_t, 2>();
}

unsigned int PokemonDB::getMoveBasePower(const unsigned int theMoveIndex) const {
    if( loaded ) {
        unsigned int offset = MOVE_OFFSET * theMoveIndex;

        uint8_t part1 = db_binary_moves.str().at(offset);
        uint8_t part2 = db_binary_moves.str().at(offset+1);

        return 256U * part2 + part1; //because it is saved in two separate bytes
    }

    else return 0;
}

unsigned int PokemonDB::getMoveBasePowerZ(const unsigned int theMoveIndex) const {
    if( loaded ) {
        unsigned int offset = MOVE_OFFSET * theMoveIndex + 5;

        uint8_t part1 = db_binary_moves.str().at(offset);
        uint8_t part2 = db_binary_moves.str().at(offset+1);

        return 256U * part2 + part1; //because it is saved in two separate bytes
    }

    else return 0;
}

bool PokemonDB::isSignatureZ(const unsigned int theMoveIndex) const {
    if( loaded ) {
        unsigned int offset = MOVE_OFFSET * theMoveIndex + 7;

        return db_binary_moves.str().at(offset);
    }

    else return 0;
}

uint8_t PokemonDB::getMoveType(const unsigned int theMoveIndex) const {
    if( loaded ) {
        unsigned int offset = MOVE_OFFSET * theMoveIndex + 2;

        return db_binary_moves.str().at(offset);
    }

    else return 18;
}

uint8_t PokemonDB::getMoveCategory(const unsigned int theMoveIndex) const {
    if( loaded ) {
        unsigned int offset = MOVE_OFFSET * theMoveIndex + 3;

        return db_binary_moves.str().at(offset);
    }

    else return 0;
}

uint8_t PokemonDB::isMoveSpread(const unsigned int theMoveIndex) const {
    if( loaded ) {
        unsigned int offset = MOVE_OFFSET * theMoveIndex + 4;

        return db_binary_moves.str().at(offset);
    }

    else return 0;
}

std::array<uint16_t, 3> PokemonDB::getPokemonAbilities(const unsigned int thePokedexNumber) const {
    if( loaded ) {
        unsigned int offset = (POKEMON_OFFSET * thePokedexNumber) + 24;

        std::array<uint16_t, 3> buffer;
        buffer[0] = (uint8_t)db_binary_species.str().at(offset); //THIS CAST IS SO STUPID
        buffer[1] = (uint8_t)db_binary_species.str().at(offset+1);
        buffer[2] = (uint8_t)db_binary_species.str().at(offset+2);

        return buffer;
    }

    else return std::array<uint16_t, 3>();
}

bool PokemonDB::isItemRemovable(const unsigned int theItemIndex) const {
    if( loaded ) {
        unsigned int offset = ITEM_OFFSET * theItemIndex;

        return db_binary_items.str().at(offset);
    }

    else return 0;
}

bool PokemonDB::isItemReducingBerry(const unsigned int theItemIndex) const {
    if( loaded ) {
        unsigned int offset = ITEM_OFFSET * theItemIndex + 1;

        return db_binary_items.str().at(offset);
    }

    else return 0;
}

uint8_t PokemonDB::getReducingBerryType(const unsigned int theItemIndex) const {
    if( loaded ) {
        unsigned int offset = ITEM_OFFSET * theItemIndex + 2;

        return db_binary_items.str().at(offset);
    }

    else return 0;
}
