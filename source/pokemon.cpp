#include <cmath>
#include <iostream>
#include <algorithm>

#include "pokemon.hpp"
#include "turn.hpp"

PokemonDB Pokemon::db;

/*TYPE CHART INITIALIZATION*/
const float Pokemon::type_matrix[18][18] = {
	{1, 1, 1, 1, 1, 0.5, 1, 0, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{2, 1, 0.5, 0.5, 1, 2, 0.5, 0, 2, 1, 1, 1, 1, 0.5, 2, 1, 2, 0.5},
	{1, 2, 1, 1, 1, 0.5, 2, 1, 0.5, 1, 1, 2, 0.5, 1, 1, 1, 1, 1},
	{1, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 0, 1, 1, 2, 1, 1, 1, 1, 1, 2},
	{1, 1, 0, 2, 1, 2, 0.5, 1, 2, 2, 1, 0.5, 2, 1, 1, 1, 1, 1},
	{1, 0.5, 2, 1, 0.5, 1, 2, 1, 0.5, 2, 1, 1, 1, 1, 2, 1, 1, 1},
	{1, 0.5, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 0.5, 1, 2, 1, 2, 1, 1, 2, 0.5},
	{0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 0.5, 1},
	{1, 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 1, 2, 1, 1, 2},
	{1, 1, 1, 1, 1, 0.5, 2, 1, 2, 0.5, 0.5, 2, 1, 1, 2, 0.5, 1, 1},
	{1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 0.5, 1, 1},
	{1, 1, 0.5, 0.5, 2, 2, 0.5, 1, 0.5, 0.5, 2, 0.5, 1, 1, 1, 0.5, 1, 1},
	{1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 0.5, 1, 1},
	{1, 2, 1, 2, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 0.5, 1, 1, 0, 1},
	{1, 1, 2, 1, 2, 1, 1, 1, 0.5, 0.5, 0.5, 2, 1, 1, 0.5, 2, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 1, 1, 2, 1, 0},
	{1, 0.5, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5},
	{1, 2, 1, 0.5, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 1, 1, 2, 2, 1},
};
/*----------------------------------------------------END------------------------------------*/

Pokemon::Pokemon(const unsigned int thePokedexNumber, const Stats& theStats) {
    current_hp_percentage = 100;

    pokedex_number = thePokedexNumber;

    base[Stats::HP] = db.getPokemonStat(thePokedexNumber, Stats::HP);
    base[Stats::ATK] = db.getPokemonStat(thePokedexNumber, Stats::ATK);
    base[Stats::DEF] = db.getPokemonStat(thePokedexNumber, Stats::DEF);
    base[Stats::SPATK] = db.getPokemonStat(thePokedexNumber, Stats::SPATK);
    base[Stats::SPDEF] = db.getPokemonStat(thePokedexNumber, Stats::SPDEF);
    base[Stats::SPE] = db.getPokemonStat(thePokedexNumber, Stats::SPE);

    types[0] = (Type)db.getPokemonType(thePokedexNumber)[0];
    types[1] = (Type)db.getPokemonType(thePokedexNumber)[1];

    possible_abilities[0] = (Ability)db.getPokemonAbilities(thePokedexNumber)[0];
    possible_abilities[1] = (Ability)db.getPokemonAbilities(thePokedexNumber)[1];
    possible_abilities[2] = (Ability)db.getPokemonAbilities(thePokedexNumber)[2];

    ability = possible_abilities[0];

    stats = theStats;

    status = Status::NO_STATUS;

    calculateTotal();
}

uint8_t Pokemon::calculateEVSNextStat(Pokemon thePokemon, const Stats::Stat& theStat, const unsigned int theStartingEVS) const {
    thePokemon.setEV(theStat, theStartingEVS);
    //auto base_evs = thePokemon.getEV(theStat);

    auto base_stat = thePokemon.getBoostedStat(theStat);

    while( thePokemon.getBoostedStat(theStat) == base_stat ) thePokemon.setEV(theStat, thePokemon.getEV(theStat) + 1);


    return thePokemon.getEV(theStat) - theStartingEVS;
}

void Pokemon::calculateTotal() {
    //calculate hp
    total[Stats::HP] = ((2 * base[Stats::HP] + stats.getIV(Stats::HP) + stats.getEV(Stats::HP)/4) * stats.getLevel())/100 + stats.getLevel() + 10;
    boosted[Stats::HP] = total[Stats::HP];

    float nature_multiplier = 1;

    //calculate atk
    float atk_modifier_multiplier;
    int16_t atk_modifier = stats.getModifier(Stats::ATK);
    if( atk_modifier == 0) atk_modifier_multiplier = 1;
    else if(atk_modifier > 0) atk_modifier_multiplier = (float(atk_modifier) + 2) / 2;
    else atk_modifier_multiplier = 2 / (float(-atk_modifier) + 2);

    if( stats.getNature() == Stats::LONELY || stats.getNature() == Stats::BRAVE || stats.getNature() == Stats::ADAMANT || stats.getNature() == Stats::NAUGHTY ) nature_multiplier = 1.1;
    else if( stats.getNature() == Stats::BOLD || stats.getNature() == Stats::TIMID || stats.getNature() == Stats::MODEST || stats.getNature() == Stats::CALM ) nature_multiplier = 0.9;
    else nature_multiplier = 1;

    total[Stats::ATK] = (((((2 * base[Stats::ATK] + stats.getIV(Stats::ATK) + stats.getEV(Stats::ATK)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    if( getItem() == Items::Choice_Band ) total[Stats::ATK] = total[Stats::ATK] * 1.5;
    boosted[Stats::ATK] = total[Stats::ATK] * atk_modifier_multiplier;

    //calculate def
    float def_modifier_multiplier;
    int16_t def_modifier = stats.getModifier(Stats::DEF);
    if( def_modifier == 0) def_modifier_multiplier = 1;
    else if(def_modifier > 0) def_modifier_multiplier = (float(def_modifier) + 2) / 2;
    else def_modifier_multiplier = 2 / (float(-def_modifier) + 2);

    if( stats.getNature() == Stats::BOLD || stats.getNature() == Stats::RELAXED || stats.getNature() == Stats::IMPISH || stats.getNature() == Stats::LAX ) nature_multiplier = 1.1;
    else if( stats.getNature() == Stats::HASTY || stats.getNature() == Stats::MILD || stats.getNature() == Stats::LONELY || stats.getNature() == Stats::GENTLE ) nature_multiplier = 0.9;
    else nature_multiplier = 1;

    total[Stats::DEF] = (((((2 * base[Stats::DEF] + stats.getIV(Stats::DEF) + stats.getEV(Stats::DEF)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    boosted[Stats::DEF] = total[Stats::DEF] * def_modifier_multiplier;

    //calculate spatk
    float spatk_modifier_multiplier;
    int16_t spatk_modifier = stats.getModifier(Stats::SPATK);
    if( spatk_modifier == 0) spatk_modifier_multiplier = 1;
    else if(spatk_modifier > 0) spatk_modifier_multiplier = (float(spatk_modifier) + 2) / 2;
    else spatk_modifier_multiplier = 2 / (float(-spatk_modifier) + 2);

    if( stats.getNature() == Stats::MODEST || stats.getNature() == Stats::QUIET|| stats.getNature() == Stats::MILD || stats.getNature() == Stats::RASH ) nature_multiplier = 1.1;
    else if( stats.getNature() == Stats::ADAMANT || stats.getNature() == Stats::IMPISH || stats.getNature() == Stats::JOLLY || stats.getNature() == Stats::CAREFUL ) nature_multiplier = 0.9;
    else nature_multiplier = 1;

    total[Stats::SPATK] = (((((2 * base[Stats::SPATK] + stats.getIV(Stats::SPATK) + stats.getEV(Stats::SPATK)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    if( getItem() == Items::Choice_Specs ) total[Stats::SPATK] = total[Stats::SPATK] * 1.5;
    boosted[Stats::SPATK] = total[Stats::SPATK] * spatk_modifier_multiplier;

    //calculate spdef
    float spdef_modifier_multiplier;
    int16_t spdef_modifier = stats.getModifier(Stats::SPDEF);
    if( spdef_modifier == 0) spdef_modifier_multiplier = 1;
    else if(spdef_modifier > 0) spdef_modifier_multiplier = (float(spdef_modifier) + 2) / 2;
    else spdef_modifier_multiplier = 2 / (float(-spdef_modifier) + 2);

    if( stats.getNature() == Stats::CALM || stats.getNature() == Stats::GENTLE || stats.getNature() == Stats::SASSY || stats.getNature() == Stats::CAREFUL ) nature_multiplier = 1.1;
    else if( stats.getNature() == Stats::NAUGHTY || stats.getNature() == Stats::LAX || stats.getNature() == Stats::NAIVE || stats.getNature() == Stats::RASH ) nature_multiplier = 0.9;
    else nature_multiplier = 1;

    total[Stats::SPDEF] = (((((2 * base[Stats::SPDEF] + stats.getIV(Stats::SPDEF) + stats.getEV(Stats::SPDEF)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    if( getItem() == Items::Assault_Vest ) total[Stats::SPDEF] = total[Stats::SPDEF] * 1.5;
    boosted[Stats::SPDEF] = total[Stats::SPDEF] * spdef_modifier_multiplier;

    //calculate spe
    float spe_modifier_multiplier;
    int16_t spe_modifier = stats.getModifier(Stats::SPE);
    if( spe_modifier == 0) spe_modifier_multiplier = 1;
    else if(spe_modifier > 0) spe_modifier_multiplier = (float(spe_modifier) + 2) / 2;
    else spe_modifier_multiplier = 2 / (float(-spe_modifier) + 2);

    if( stats.getNature() == Stats::TIMID || stats.getNature() == Stats::HASTY || stats.getNature() == Stats::JOLLY || stats.getNature() == Stats::NAIVE ) nature_multiplier = 1.1;
    else if( stats.getNature() == Stats::BRAVE || stats.getNature() == Stats::RELAXED || stats.getNature() == Stats::QUIET || stats.getNature() == Stats::SASSY ) nature_multiplier = 0.9;
    else nature_multiplier = 1;

    total[Stats::SPE] = (((((2 * base[Stats::SPE] + stats.getIV(Stats::SPE) + stats.getEV(Stats::SPE)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    boosted[Stats::SPE] = total[Stats::SPE] * spe_modifier_multiplier;
}

float Pokemon::calculateWeatherModifier(const Move& theMove) const {
    if( theMove.getWeather() == Move::RAIN && getAbility() != Ability::Cloud_Nine && getAbility() != Ability::Air_Lock ) {
        if( theMove.getMoveType() == Water ) return 1.5;
        else if( theMove.getMoveType() == Fire ) return 0.5;
        else return 1;
    }

    else if( theMove.getWeather() == Move::SUN && getAbility() != Ability::Cloud_Nine && getAbility() != Ability::Air_Lock ) {
        if( theMove.getMoveType() == Water ) return 0.5;
        else if( theMove.getMoveType() == Fire ) return 1.5;
        else return 1;
    }

    else return 1;
}

float Pokemon::calculateStabModifier(const Pokemon& theAttacker, const Move& theMove) const {
    float stab_modifier;
    if( theAttacker.getAbility() == Ability::Adaptability ) stab_modifier = 2;
    else stab_modifier = 1.5;

    if( (theAttacker.getTypes()[0] == theMove.getMoveType()) || (theAttacker.getTypes()[1] == theMove.getMoveType())) return stab_modifier;
    else return 1;
}

float Pokemon::calculateTargetModifier(const Move& theMove) const {
    if( theMove.getTarget() == Move::SINGLE || theMove.isZ() ) return 1;
    else return 0.75;
}

float Pokemon::calculateCritModifier(const Move& theMove) const {
    if( theMove.isCrit() ) return 1.5;
    else return 1;
}

float Pokemon::calculateBurnModifier(const Pokemon& theAttacker, const Move& theMove) const {
    if( theAttacker.getStatus() == Status::BURNED && theMove.getMoveCategory() == Move::PHYSICAL ) return 0.5;
    else return 1;
}

float Pokemon::calculateTypeModifier(const Move& theMove) const {
    if( getTypes()[0] == getTypes()[1] ) return type_matrix[theMove.getMoveType()][getTypes()[0]];
    else return type_matrix[theMove.getMoveType()][getTypes()[0]] * type_matrix[theMove.getMoveType()][getTypes()[1]];
}

float Pokemon::calculateOtherModifier(const Pokemon& theAttacker, const Move& theMove) const {
    float modifier = 1;

    if( getAbility() == Ability::Shadow_Shield && getCurrentHPPercentage() == 100 ) modifier = modifier * 0.5;
    if( getAbility() == Ability::Prism_Armor && calculateTypeModifier(theMove) >= 2 ) modifier = modifier * 0.75;
    if( getAbility() == Ability::Wonder_Guard && calculateTypeModifier(theMove) < 2 ) modifier = modifier * 0;
    if( theAttacker.getItem() == Items::Life_Orb ) modifier = modifier * 1.3;

    return modifier;
}

uint16_t Pokemon::calculateDefenseInMove(const Move& theMove) const {
    uint16_t defense;

    if( theMove.isCrit() ) {
        if( theMove.getMoveCategory() == Move::PHYSICAL || (theMove.getMoveIndex() == Moves::Psyshock && !theMove.isZ()) ) {
            if( (getModifier(Stats::DEF) > 0) || (theMove.getMoveIndex() == Moves::Darkest_Lariat && !theMove.isZ()) || (theMove.getMoveIndex() == Moves::Sacred_Sword && !theMove.isZ()) ) defense = getStat(Stats::DEF);
            else defense = getBoostedStat(Stats::DEF);
        }

        else {
            if( getModifier(Stats::SPDEF) > 0 ) defense = getStat(Stats::SPDEF);
            else defense = getBoostedStat(Stats::SPDEF);
        }
    }

    else {
        if( theMove.getMoveCategory() == Move::PHYSICAL || (theMove.getMoveIndex() == Moves::Psyshock && !theMove.isZ()) ) defense = getBoostedStat(Stats::DEF);
        else if( (theMove.getMoveIndex() == Moves::Darkest_Lariat && !theMove.isZ()) || (theMove.getMoveIndex() == Moves::Sacred_Sword && !theMove.isZ()) ) defense = getStat(Stats::DEF);
        else defense = getBoostedStat(Stats::SPDEF);
    }

    return defense;
}

uint16_t Pokemon::calculateAttackInMove(const Pokemon& theAttacker, const Move& theMove) const {
    uint16_t attack;

    if( theMove.isCrit() ) {
        if( theMove.getMoveCategory() == Move::PHYSICAL ) {
            if( theMove.getMoveIndex() == Moves::Foul_Play && !theMove.isZ() ) {
                if( getModifier(Stats::ATK) < 0 ) attack = getStat(Stats::ATK);
                else attack = getBoostedStat(Stats::ATK);
            }

            else if( theAttacker.getModifier(Stats::ATK) < 0 ) attack = theAttacker.getStat(Stats::ATK);
            else attack = theAttacker.getBoostedStat(Stats::ATK);
        }

        else {
            if( theAttacker.getModifier(Stats::SPATK) < 0 ) attack = theAttacker.getStat(Stats::SPATK);
            else attack = theAttacker.getBoostedStat(Stats::SPATK);
        }
    }

    else {
        if( theMove.getMoveIndex() == Moves::Foul_Play && !theMove.isZ() ) attack = getBoostedStat(Stats::ATK);
        else if( theMove.getMoveCategory() == Move::PHYSICAL ) attack = theAttacker.getBoostedStat(Stats::ATK);
        else attack = theAttacker.getBoostedStat(Stats::SPATK);
    }

    if( theAttacker.getAbility() == Ability::Blaze && theAttacker.getCurrentHPPercentage() <= float(100/3) && theMove.getMoveType() == Type::Fire ) attack = attack * 1.5;
    if( theAttacker.getAbility() == Ability::Overgrow && theAttacker.getCurrentHPPercentage() <= float(100/3) && theMove.getMoveType() == Type::Grass  ) attack = attack * 1.5;

    return attack;
}

unsigned int Pokemon::calculateMoveBasePowerInAttack(const Pokemon& theAttacker, const Move& theMove) const {
    unsigned int bp;

    if( theMove.isZ() ) bp = theMove.getZBasePower();

    else bp = theMove.getBasePower();

    if( theAttacker.getAbility() == Ability::Technician && bp <= 60 ) bp = bp * 1.5;
    if( getAbility() == Dry_Skin && theMove.getMoveType() == Type::Fire ) bp = bp * 1.25;
    if( (getAbility() == Dark_Aura || theMove.isDarkAura()) && theMove.getMoveType() == Type::Dark ) bp = bp * 1.33;
    if( (getAbility() == Fairy_Aura || theMove.isFairyAura()) && theMove.getMoveType() == Type::Fairy ) bp = bp * 1.33;
    if( theMove.getMoveIndex() == Moves::Acrobatics && theAttacker.getItem() == Items::None ) bp = bp * 2;
    if( !theMove.isZ() && theMove.getMoveIndex() == Moves::Facade && (theAttacker.getStatus() == Status::BURNED || theAttacker.getStatus() == Status::POISONED || theAttacker.getStatus() == PARALYZED) ) bp = bp * 2;
    //if( theMove.getMoveIndex() == Moves::Knock_Off ) bp = bp * 1.5;
    if( !theMove.isZ() && theMove.getMoveIndex() == Moves::Brine && getCurrentHPPercentage() <= 50 ) bp = bp * 2;
    if( !theMove.isZ() && (theMove.getMoveIndex() == Moves::Water_Spout || theMove.getMoveIndex() == Moves::Eruption) ) {
        bp = (theAttacker.getCurrentHP() * 150) / theAttacker.getStat(Stats::HP);
        if( bp < 1 ) return 1;
        else return bp;
    }

    return bp;
}

std::vector<unsigned int> Pokemon::getDamage(const Pokemon& theAttacker, Move theMove) const {
    uint16_t defense;
    uint16_t attack;

    attack = calculateAttackInMove(theAttacker, theMove);
    defense = calculateDefenseInMove(theMove);

    theMove.setBasePower(calculateMoveBasePowerInAttack(theAttacker, theMove));

    unsigned int base_damage = (floor(floor((floor((2*getLevel())/5)+2) * theMove.getBasePower() * attack/defense) / 50) + 2);
    if( base_damage == 0 ) base_damage = 1;

    //calculating move modifiers
    theMove.setModifier().setWeatherModifier(calculateWeatherModifier(theMove)); //WEATHER
    theMove.setModifier().setStabModifier(calculateStabModifier(theAttacker, theMove)); //STAB
    theMove.setModifier().setTargetModifier(calculateTargetModifier(theMove)); //TARGET
    theMove.setModifier().setCritModifier(calculateCritModifier(theMove)); //CRIT
    theMove.setModifier().setBurnModifier(calculateBurnModifier(theAttacker, theMove)); //BURN
    theMove.setModifier().setTypeModifier(calculateTypeModifier(theMove)); //TYPE
    theMove.setModifier().setOtherModifier(calculateOtherModifier(theAttacker, theMove)); //OTHER

    std::vector<unsigned int> buffer;
    for(float mod = 0.85; mod <= 1; mod = mod + 0.01 ) {
        Modifier modifier = theMove.getModifier();
        if( mod > 0.99 ) modifier.setRandomModifier(1); //THIS IS STUPID BUT I CAN'T FIGURE OUT WHY IT DOESN'T WORK WITHOUT THIS
        else modifier.setRandomModifier(mod);
        std::array<float, Modifier::MOD_NUM> modifiers = modifier.getModifiers();

        unsigned int damage = base_damage;
        for(unsigned int i = 0; i < modifiers.size(); i++) damage = floor(damage * modifiers[i]);
        buffer.push_back(damage);
    }

    return buffer;
}

std::vector<float> Pokemon::getDamagePercentage(const Turn& theTurn) const {
    std::vector<unsigned int> damages = getDamageInt(theTurn);
    std::vector<float> buffer;
    for(unsigned int i = 0; i < damages.size(); i++) buffer.push_back(damages[i] / float(getCurrentHP()) * 100);

    return buffer;
}

void Pokemon::recursiveDamageCalculation(std::vector<unsigned int>& theUintVector, std::vector<std::pair<Pokemon, Move>>& theVector, std::vector<std::pair<Pokemon, Move>>::iterator& it) const {
    if( theUintVector.empty() && it == theVector.begin() ) {
        theUintVector = getDamage(it->first, it->second);
        it++;
        recursiveDamageCalculation(theUintVector, theVector, it);
    }

    else if( it == theVector.end() ) return;

    else {
        std::vector<unsigned int> buffer;
        std::vector<unsigned int> damages = getDamage(it->first, it->second);

        for(unsigned int i = 0; i < damages.size(); i++) {
            unsigned int value = damages[i];
            for(unsigned int j = 0; j < theUintVector.size(); j++) {
                buffer.push_back(value + theUintVector[j]);
            }
        }

        theUintVector = buffer;
        it++;

    }
}

std::vector<unsigned int> Pokemon::getDamageInt(const Turn& theTurn) const {
    std::vector<std::pair<Pokemon, Move>> buffer = theTurn.getMoves();
    std::vector<std::pair<Pokemon, Move>>::iterator it = buffer.begin();
    std::vector<unsigned int> vec;

    recursiveDamageCalculation(vec, buffer, it);
    return vec;
}

float Pokemon::getKOProbability(const Turn& theTurn) const {
    std::vector<float> damages = getDamagePercentage(theTurn);

    unsigned int ko_count = 0;
    for(auto it = damages.begin(); it < damages.end(); it++) if( *it >= float(100 / theTurn.getHits()) ) ko_count++;

    return (float(ko_count) / damages.size()) * 100;
}


std::tuple<int, int, int> Pokemon::resistMove(const std::vector<Turn>& theTurn, const std::vector<defense_modifier>& theDefModifiers, std::vector<float>& theKoProbability) {
    if( theTurn.size() != theDefModifiers.size() ) return std::make_tuple(-1, -1, -1);
    if( theTurn.empty() ) return std::make_tuple(-1, -1, -1);

    auto results = resistMoveLoop(theTurn, theDefModifiers);

    if( results.empty() ) return std::make_tuple(-1, -1, -1);

    //finding all the minimum elements
    std::vector<unsigned int> sum_results;
    //creating the vector with the sums
    for( auto it = results.begin(); it < results.end(); it++ ) sum_results.push_back(std::get<0>(*it)+std::get<1>(*it)+std::get<2>(*it));
    //finding the minimum sums
    auto min_index = std::min_element(sum_results.begin(), sum_results.end());
    std::vector<std::vector<unsigned int>::iterator> final_results;
    //all of them
    for(auto it = sum_results.begin(); it < sum_results.end(); it++) if( *it == *min_index ) final_results.push_back(it);

    //finding the one with the max hp
    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> returnable_results;
    for(auto it = final_results.begin(); it < final_results.end(); it++) returnable_results.push_back(results[std::distance(sum_results.begin(), *it)]);

    std::tuple<uint8_t, uint8_t, uint8_t> final_pair = returnable_results[0];
    for(auto it = returnable_results.begin(); it < returnable_results.end(); it++)
        if( std::get<0>(*it) >= std::get<0>(final_pair) ) final_pair = *it;

    theKoProbability.clear();
    Pokemon buffer = *this;
    buffer.setEV(Stats::HP, std::get<0>(final_pair));
    buffer.setEV(Stats::DEF, std::get<1>(final_pair));
    buffer.setEV(Stats::SPDEF, std::get<2>(final_pair));

    for( auto it = theTurn.begin(); it < theTurn.end(); it++ ) theKoProbability.push_back(buffer.getKOProbability(*it));

    return final_pair;
}

int Pokemon::outspeedPokemon(const std::vector<Pokemon>& theVector) {
    const unsigned int MAX_EVS = 510;
    const unsigned int MAX_EVS_SINGLE_STAT = 252;

    Pokemon defender = *this;

    unsigned int assignable_evs = MAX_EVS - (defender.getEV(Stats::ATK) + defender.getEV(Stats::SPATK) + defender.getEV(Stats::DEF) + defender.getEV(Stats::SPDEF) + defender.getEV(Stats::HP));
    defender.setAllEV(0, 0, 0, 0, 0, 0);

    std::vector<uint16_t> speed_stats;
    for(auto it = theVector.begin(); it < theVector.end(); it++) speed_stats.push_back(it->getBoostedStat(Stats::SPE));

    uint16_t speed_max = *std::max_element(speed_stats.begin(), speed_stats.end());

    for(unsigned int i = 0; i < MAX_EVS_SINGLE_STAT; i++) {
        defender.setEV(Stats::SPE, i);
        if( defender.getBoostedStat(Stats::SPE) > speed_max && i <= assignable_evs ) return i;
    }

    return -1;
}

std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> Pokemon::resistMoveLoop(const std::vector<Turn>& theTurn, const std::vector<defense_modifier>& theDefModifiers) const {
    const unsigned int MAX_EVS = 510;
    const unsigned int MAX_EVS_SINGLE_STAT = 252;

    Pokemon defender = *this;

        //calculating if we could use the faster loop
    Turn::Type previous = theTurn.begin()->getType();
    bool simplified = true;
    for( auto it = theTurn.begin(); it < theTurn.end(); it++ ) {
        if( it->getType() == Turn::MIXED || it->getType() != previous ) simplified = false;
        previous = it->getType();
    }

    Move::Category simplified_type = Move::SPECIAL; //just random assignment
    if( simplified ) {
        if( previous == Turn::PHYSICAL ) simplified_type = Move::PHYSICAL;
        else simplified_type = Move::SPECIAL;
    }

    unsigned int assignable_evs = MAX_EVS - (defender.getEV(Stats::ATK) + defender.getEV(Stats::SPATK) + defender.getEV(Stats::SPE));
    defender.setAllEV(0, 0, 0, 0, 0, 0);

    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> results;
    unsigned int roll_count = 0;

    while( results.empty() && roll_count < 6) {
        float tolerance = (5 * roll_count);

        for(unsigned int hp_assigned = 0; hp_assigned < MAX_EVS_SINGLE_STAT + 1; hp_assigned = hp_assigned + calculateEVSNextStat(defender, Stats::HP, hp_assigned)) {
            //std::cout << floor(float(hp_assigned) / float(MAX_EVS_SINGLE_STAT) * 100) << "\n";
            defender.setEV(Stats::HP, hp_assigned);

            for(unsigned int spdef_assigned = 0; spdef_assigned < MAX_EVS_SINGLE_STAT + 1; spdef_assigned = spdef_assigned + calculateEVSNextStat(defender, Stats::SPDEF, spdef_assigned)) {
                if( simplified && simplified_type == Move::PHYSICAL && spdef_assigned > 0 ) break;
                defender.setEV(Stats::SPDEF, spdef_assigned);

                for(unsigned int def_assigned = 0; def_assigned < MAX_EVS_SINGLE_STAT + 1; def_assigned = def_assigned + calculateEVSNextStat(defender, Stats::DEF, def_assigned)) {
                    if( simplified && simplified_type == Move::SPECIAL && def_assigned > 0 ) break;
                    defender.setEV(Stats::DEF, def_assigned);

                    bool to_add = true;
                    for(unsigned int it = 0; it < theTurn.size() && to_add; it++ ) {
                        defender.setCurrentHPPercentage(std::get<0>(theDefModifiers[it]));
                        defender.setModifier(Stats::DEF, std::get<1>(theDefModifiers[it]));
                        defender.setModifier(Stats::SPDEF, std::get<2>(theDefModifiers[it]));
                        if( defender.getKOProbability(theTurn[it]) > (0 + tolerance) || (hp_assigned + def_assigned + spdef_assigned > assignable_evs) ) to_add = false;
                    }

                    if( to_add ) results.push_back(std::make_tuple(hp_assigned, def_assigned, spdef_assigned));
                }
            }
        }

        roll_count++;
    }

    return results;
}
