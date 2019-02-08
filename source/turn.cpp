#include "turn.hpp"

Turn::Turn() {
    hits = 1;
    type = TYPE_NONE;
}

void Turn::addMove(const Pokemon& thePokemon, const Move& theMove) {
    if( moves.empty() ) {
        if( theMove.getMoveCategory() == Move::PHYSICAL ) type = PHYSICAL;
        else if( theMove.getMoveCategory() == Move::SPECIAL) type = SPECIAL;
        else type = TYPE_NONE;
    }

    else {
        Type new_type;
        if( theMove.getMoveCategory() == Move::PHYSICAL ) new_type = PHYSICAL;
        else if( theMove.getMoveCategory() == Move::SPECIAL) new_type = SPECIAL;
        else new_type = TYPE_NONE;

        if( new_type != type ) type = MIXED;
    }

    moves.push_back(std::make_pair(thePokemon, theMove));

    //ACCOUNTING FOR WEATHER CANCELATION
    if( thePokemon.getAbility() == Ability::Cloud_Nine || thePokemon.getAbility() == Ability::Air_Lock )
        for(auto it = moves.begin(); it < moves.end(); it++)
            it->second.setWeather(Move::WEATHER_NONE);

    //ACCOUNTING FOR FAIRY AND DARK AURA
    if( thePokemon.getAbility() == Ability::Fairy_Aura )
        for(auto it = moves.begin(); it < moves.end(); it++)
            it->second.setFairyAura(true);

    if( thePokemon.getAbility() == Ability::Dark_Aura )
        for(auto it = moves.begin(); it < moves.end(); it++)
            it->second.setDarkAura(true);
}

std::vector<std::pair<Pokemon, Move>> Turn::getMovesEffective() const {
    std::vector<std::pair<Pokemon, Move>> buffer;

    for(unsigned int i = 0; i < hits; i++)
        for( auto it = moves.begin(); it < moves.end(); it++ )
            buffer.push_back(*it);

    return buffer;

}
