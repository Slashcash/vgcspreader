#include "pokemondb.hpp"

PokemonDB::PokemonDB() {
    loaded = false;

    //should we launch an exception if something goes wrong here? Probably so
    loaded = load();
}

bool PokemonDB::load() {
    if( !loaded ) {
        Q_INIT_RESOURCE(resources);

        //loading the species binary
        QFile input_file_species(":/db/personal_species.bin");
        if( !input_file_species.open(QIODevice::ReadOnly) ) return false;
        species_data = input_file_species.readAll();

        //loading the moves binary
        QFile input_file_moves(":/db/personal_moves.bin");
        if( !input_file_moves.open(QIODevice::ReadOnly) ) return false;
        moves_data = input_file_moves.readAll();

        //loading the items binary
        QFile input_file_items(":/db/personal_items.bin");
        if( !input_file_items.open(QIODevice::ReadOnly) ) return false;
        items_data = input_file_items.readAll();

        return true;
    }

    else return true;
}

std::array<Type, 2> PokemonDB::getPokemonType(const unsigned int thePokedexNumber, const unsigned int theForm) const {
    auto buffer = readSpeciesData(getPokemonOffset(thePokedexNumber, theForm) + TYPE_OFFSET, TYPE_SIZE);
    std::array<Type, 2> result;
    result[0] = static_cast<Type>(buffer[0]);
    result[1] = static_cast<Type>(buffer[1]);

    return result;
}

unsigned int PokemonDB::getMoveBasePower(const unsigned int theMoveIndex) const {
    auto buffer = readMovesData(getMoveOffset(theMoveIndex) + MOVEBP_OFFSET, MOVEBP_SIZE);

    return 256U * buffer[1] + buffer[0]; //because it is saved in two separate bytes
}

unsigned int PokemonDB::getMoveBasePowerZ(const unsigned int theMoveIndex) const {
    auto buffer = readMovesData(getMoveOffset(theMoveIndex) + MOVEBPZ_OFFSET, MOVEBPZ_SIZE);

    return 256U * buffer[1] + buffer[0]; //because it is saved in two separate bytes
}

std::array<Ability, 3> PokemonDB::getPokemonAbilities(const unsigned int thePokedexNumber, const unsigned int theForm) const {
    auto buffer = readSpeciesData(getPokemonOffset(thePokedexNumber, theForm) + ABILITY_OFFSET, ABILITY_SIZE);
    std::array<Ability, 3> result;
    result[0] = static_cast<Ability>(buffer[0]);
    result[1] = static_cast<Ability>(buffer[1]);
    result[2] = static_cast<Ability>(buffer[2]);

    return result;
}

std::vector<uint8_t> PokemonDB::readData(const QByteArray& theData, const unsigned int theOffset, const unsigned int theSize) const {
    if( loaded ) {
        std::vector<uint8_t> buffer;
        for(unsigned int i = theOffset; i < theOffset + theSize; i++) buffer.push_back(static_cast<uint8_t>(theData.at(static_cast<int>(i))));
        
        return buffer;
    }
    
    else return std::vector<uint8_t>();
}

unsigned int PokemonDB::getPokemonOffset(const unsigned int thePokedexNumber, const unsigned int theForm) const {
    unsigned int offset;
    if( theForm == 0) offset = POKEMON_OFFSET * thePokedexNumber;
    
    else {
        unsigned int form_offset = POKEMON_OFFSET * thePokedexNumber + FORM_OFFSET;
        std::vector<uint8_t> form_buffer = readSpeciesData(form_offset, FORM_SIZE);
        
        unsigned int form_index = 256U * form_buffer[1] + form_buffer[0];
        offset = (form_index * POKEMON_OFFSET) + ((theForm-1) * POKEMON_OFFSET);
    }
    
    return offset;
}
