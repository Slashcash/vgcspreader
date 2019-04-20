#include <cmath>
#include <iostream>
#include <algorithm>
#include <thread>
#include <iterator>

#include "pokemon.hpp"
#include "turn.hpp"
#include "move.hpp"

#include <QDebug>

PokemonDB Pokemon::db;
std::mutex Pokemon::buffer_mutex;
std::mutex Pokemon::result_mutex;

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
    abort_calculation = false;

    pokedex_number = thePokedexNumber;

    formes_number = db.getPokemonFormesNumber(thePokedexNumber);
    form = 0;

    for(unsigned int i = 0; i < formes_number; i++) {
        std::array<uint8_t, 6> buffer;

        buffer[Stats::HP] = db.getPokemonStat(thePokedexNumber, i, Stats::HP);
        buffer[Stats::ATK] = db.getPokemonStat(thePokedexNumber, i, Stats::ATK);
        buffer[Stats::DEF] = db.getPokemonStat(thePokedexNumber, i, Stats::DEF);
        buffer[Stats::SPATK] = db.getPokemonStat(thePokedexNumber, i, Stats::SPATK);
        buffer[Stats::SPDEF] = db.getPokemonStat(thePokedexNumber, i, Stats::SPDEF);
        buffer[Stats::SPE] = db.getPokemonStat(thePokedexNumber, i, Stats::SPE);

        base.push_back(buffer);
    }

    for(unsigned int i = 0; i < formes_number; i++) {
        std::array<Type, 2> buffer;

        buffer[0] = (Type)db.getPokemonType(thePokedexNumber, i)[0];
        buffer[1] = (Type)db.getPokemonType(thePokedexNumber, i)[1];

        types.push_back(buffer);
    }

    for(unsigned int i = 0; i < formes_number; i++) {
        std::array<Ability, 3> buffer;

        buffer[0] = (Ability)db.getPokemonAbilities(thePokedexNumber, i)[0];
        buffer[1] = (Ability)db.getPokemonAbilities(thePokedexNumber, i)[1];
        buffer[2] = (Ability)db.getPokemonAbilities(thePokedexNumber, i)[2];

        possible_abilities.push_back(buffer);
    }

    ability = possible_abilities[form][0];

    stats = theStats;

    status = Status::NO_STATUS;

    calculateTotal();

    //evaluate if it is grounded
    if( types[form][0] == Type::Flying || types[form][1] == Type::Flying || getAbility() == Ability::Levitate ) grounded = false;
    else grounded = true;
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
    total[Stats::HP] = ((2 * base[form][Stats::HP] + stats.getIV(Stats::HP) + stats.getEV(Stats::HP)/4) * stats.getLevel())/100 + stats.getLevel() + 10;
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

    total[Stats::ATK] = (((((2 * base[form][Stats::ATK] + stats.getIV(Stats::ATK) + stats.getEV(Stats::ATK)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    if( getItem() == Items::Choice_Band ) total[Stats::ATK] = total[Stats::ATK] * 1.5;
    if( getAbility() == Ability::Huge_Power ) total[Stats::ATK] = total[Stats::ATK] * 2;
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

    total[Stats::DEF] = (((((2 * base[form][Stats::DEF] + stats.getIV(Stats::DEF) + stats.getEV(Stats::DEF)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    if( getAbility() == Ability::Fur_Coat ) total[Stats::DEF] = total[Stats::DEF] * 2;
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

    total[Stats::SPATK] = (((((2 * base[form][Stats::SPATK] + stats.getIV(Stats::SPATK) + stats.getEV(Stats::SPATK)/4) * stats.getLevel())/100)+5) * nature_multiplier);
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

    total[Stats::SPDEF] = (((((2 * base[form][Stats::SPDEF] + stats.getIV(Stats::SPDEF) + stats.getEV(Stats::SPDEF)/4) * stats.getLevel())/100)+5) * nature_multiplier);
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

    total[Stats::SPE] = (((((2 * base[form][Stats::SPE] + stats.getIV(Stats::SPE) + stats.getEV(Stats::SPE)/4) * stats.getLevel())/100)+5) * nature_multiplier);
    boosted[Stats::SPE] = total[Stats::SPE] * spe_modifier_multiplier;

    //evaluate if it is grounded
    if( types[form][0] == Type::Flying || types[form][1] == Type::Flying || getAbility() == Ability::Levitate ) grounded = false;
    else grounded = true;
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

    else if( theMove.getWeather() == Move::HARSH_SUNSHINE && getAbility() != Ability::Cloud_Nine && getAbility() != Ability::Air_Lock ) {
        if( theMove.getMoveType() == Water ) return 0;
        else if( theMove.getMoveType() == Fire ) return 1.5;
        else return 1;
    }

    else if( theMove.getWeather() == Move::HEAVY_RAIN && getAbility() != Ability::Cloud_Nine && getAbility() != Ability::Air_Lock ) {
        if( theMove.getMoveType() == Water ) return 1.5;
        else if( theMove.getMoveType() == Fire ) return 0;
        else return 1;
    }

    else return 1;
}

float Pokemon::calculateTerrainModifier(const Pokemon& theAttacker, const Move& theMove) const {
    if( theAttacker.isGrounded() ) {
        if( theMove.getTerrain() == Move::Terrain::GRASSY && theMove.getMoveType() == Type::Grass ) return 1.5;
        else if( theMove.getTerrain() == Move::Terrain::PSYCHIC && theMove.getMoveType() == Type::Psychic ) return 1.5;
        else if( theMove.getTerrain() == Move::Terrain::ELECTRIC && theMove.getMoveType() == Type::Electric ) return 1.5;
        else if( theMove.getTerrain() == Move::Terrain::MISTY && theMove.getMoveType() == Type::Dragon ) return 0.5;
        else return 1;
    }

    else return 1;
}

float Pokemon::calculateStabModifier(const Pokemon& theAttacker, const Move& theMove) const {
    float stab_modifier;
    if( theAttacker.getAbility() == Ability::Adaptability ) stab_modifier = 2;
    else stab_modifier = 1.5;

    if( (theAttacker.getTypes()[theAttacker.getForm()][0] == theMove.getMoveType()) || (theAttacker.getTypes()[theAttacker.getForm()][1] == theMove.getMoveType())) return stab_modifier;
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

float Pokemon::calculateTypeModifier(const Pokemon& theAttacker, const Move& theMove) const {
    //i should probably re-engineer this function

    //taking into account the strong winds if enabled
    if( theMove.getWeather() == Move::STRONG_WINDS && getAbility() != Ability::Cloud_Nine && getAbility() != Ability::Air_Lock ) {
        //single type
        if( getTypes()[form][0] == getTypes()[form][1] ) {
            if( getTypes()[form][0] == Type::Flying && type_matrix[theMove.getMoveType()][Type::Flying] > 1 ) return 1;
            else return type_matrix[theMove.getMoveType()][getTypes()[form][0]];
        }

        //dual type
        else {
            float part_1;
            if( getTypes()[form][0] == Type::Flying && type_matrix[theMove.getMoveType()][Type::Flying] > 1 ) part_1 = 1;
            else part_1 = type_matrix[theMove.getMoveType()][getTypes()[form][0]];

            float part_2;
            if( getTypes()[form][1] == Type::Flying && type_matrix[theMove.getMoveType()][Type::Flying] > 1 ) part_2 = 1;
            else part_2 = type_matrix[theMove.getMoveType()][getTypes()[form][1]];

            return part_1 * part_2;
        }
    }

    //in case it is scrappy
    else if( theAttacker.getAbility() == Ability::Scrappy ) {
        float part_1;
        float part_2;

        if( getTypes()[form][0] == Type::Ghost && (theMove.getMoveType() == Type::Normal || theMove.getMoveType() == Type::Fighting) ) part_1 = 1;
        else part_1 = type_matrix[theMove.getMoveType()][getTypes()[form][0]];

        if( getTypes()[form][1] == Type::Ghost && (theMove.getMoveType() == Type::Normal || theMove.getMoveType() == Type::Fighting) ) part_2 = 1;
        else part_2 = type_matrix[theMove.getMoveType()][getTypes()[form][1]];

        if( getTypes()[form][0] == getTypes()[form][1] ) return part_1;
        else return part_1 * part_2;
    }

    else {
        if( getTypes()[form][0] == getTypes()[form][1] ) return type_matrix[theMove.getMoveType()][getTypes()[form][0]];
        else return type_matrix[theMove.getMoveType()][getTypes()[form][0]] * type_matrix[theMove.getMoveType()][getTypes()[form][1]];
    }
}

float Pokemon::calculateOtherModifier(const Pokemon& theAttacker, const Move& theMove) const {
    float modifier = 1;

    //accounting for the reducing berry
    if( (getItem().isReducingBerry() && getItem().getReducingBerryType() == theMove.getMoveType() && calculateTypeModifier(theAttacker, theMove) >= 2) || ( getItem().isReducingBerry() && getItem().getReducingBerryType() == Type::Normal && theMove.getMoveType() == Type::Normal ) ) modifier = modifier * 0.5;

    if( getAbility() == Ability::Shadow_Shield && getCurrentHPPercentage() == 100 ) modifier = modifier * 0.5;
    else if( getAbility() == Ability::Prism_Armor && calculateTypeModifier(theAttacker, theMove) >= 2 ) modifier = modifier * 0.75;
    else if( theAttacker.getAbility() == Ability::Neuroforce && calculateTypeModifier(theAttacker, theMove) >= 2 ) modifier = modifier * 1.25;

    //these effects are ignored by solgaleo & lunala peculiar moves and zekrom e reshiram abilities
    if( theMove.getMoveIndex() != Moves::Moongeist_Beam && theMove.getMoveIndex() != Moves::Sunsteel_Strike && theMove.getMoveIndex() != Moves::Menacing_Moonraze_Maelstrom && theMove.getMoveIndex() != Moves::Searing_Sunraze_Smash && theAttacker.getAbility() != Ability::Turboblaze && theAttacker.getAbility() != Ability::Teravolt) {
        if( getAbility() == Ability::Wonder_Guard && calculateTypeModifier(theAttacker, theMove) < 2 ) modifier = modifier * 0;
        else if( getAbility() == Ability::Multiscale ) modifier = modifier * 0.5;
        else if( (getAbility() == Ability::Filter || getAbility() == Ability::Solid_Rock) && calculateTypeModifier(theAttacker, theMove) > 2  ) modifier = modifier * 0.75;
        else if( getAbility() == Ability::Levitate && theMove.getMoveType() == Type::Ground ) modifier = modifier * 0;
        else if( getAbility() == Ability::Heatproof && theMove.getMoveType() == Type::Fire ) modifier = modifier * 0.5;
    }

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

void Pokemon::calculateMoveTypeInAttack(const Pokemon& theAttacker, Move& theMove) const {
    //accounting for the *late ability move type change
    if( theAttacker.getAbility() == Ability::Aerilate && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) theMove.setMoveType(Type::Flying);
    if( theAttacker.getAbility() == Ability::Pixilate && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) theMove.setMoveType(Type::Fairy);
    if( theAttacker.getAbility() == Ability::Normalize && !theMove.isZ() ) theMove.setMoveType(Type::Normal);
    if( theAttacker.getAbility() == Ability::Refrigerate && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) theMove.setMoveType(Type::Ice);
    if( theAttacker.getAbility() == Ability::Galvanize && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) theMove.setMoveType(Type::Electric);

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
    if( theMove.getMoveIndex() == Moves::Knock_Off && getItem().isRemovable() && !theMove.isZ() ) bp = bp * 1.5;
    if( !theMove.isZ() && theMove.getMoveIndex() == Moves::Brine && getCurrentHPPercentage() <= 50 ) bp = bp * 2;
    if( !theMove.isZ() && (theMove.getMoveIndex() == Moves::Water_Spout || theMove.getMoveIndex() == Moves::Eruption) ) {
        bp = (theAttacker.getCurrentHP() * 150) / theAttacker.getStat(Stats::HP);
        if( bp < 1 ) return 1;
        else return bp;
    }

    //*late abilities boost
    if( theAttacker.getAbility() == Ability::Aerilate && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) bp = bp * 1.2;
    if( theAttacker.getAbility() == Ability::Pixilate && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) bp = bp * 1.2;
    if( theAttacker.getAbility() == Ability::Normalize && !theMove.isZ() ) bp = bp * 1.2;
    if( theAttacker.getAbility() == Ability::Refrigerate && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) bp = bp * 1.2;
    if( theAttacker.getAbility() == Ability::Galvanize && theMove.getMoveType() == Type::Normal && !theMove.isZ() ) bp = bp * 1.2;

    return bp;
}

std::vector<int> Pokemon::getDamage(const Pokemon& theAttacker, Move theMove) const {
    uint16_t defense;
    uint16_t attack;

    attack = calculateAttackInMove(theAttacker, theMove);
    defense = calculateDefenseInMove(theMove);

    theMove.setBasePower(calculateMoveBasePowerInAttack(theAttacker, theMove));

    unsigned int base_damage = floor(floor((floor((2 * getLevel()) / 5 + 2) * theMove.getBasePower() * attack) / defense) / 50 + 2);
    if( base_damage == 0 ) base_damage = 1;

    //changing the move type before anything else if needed (taking into account the *late abilities for example)
    calculateMoveTypeInAttack(theAttacker, theMove);

    //calculating move modifiers
    theMove.setModifier().setWeatherModifier(calculateWeatherModifier(theMove)); //WEATHER
    theMove.setModifier().setTerrainModifier(calculateTerrainModifier(theAttacker, theMove)); //TERRAIN
    theMove.setModifier().setStabModifier(calculateStabModifier(theAttacker, theMove)); //STAB
    theMove.setModifier().setTargetModifier(calculateTargetModifier(theMove)); //TARGET
    theMove.setModifier().setCritModifier(calculateCritModifier(theMove)); //CRIT
    theMove.setModifier().setBurnModifier(calculateBurnModifier(theAttacker, theMove)); //BURN
    theMove.setModifier().setTypeModifier(calculateTypeModifier(theAttacker, theMove)); //TYPE
    theMove.setModifier().setOtherModifier(calculateOtherModifier(theAttacker, theMove)); //OTHER

    std::vector<int> buffer;
    for(int mod = 15; mod >= 0; mod-- ) {
        Modifier modifier = theMove.getModifier();
        modifier.setRandomModifier(std::floor((100-mod)/100));
        std::array<float, Modifier::MOD_NUM> modifiers = modifier.getModifiers();

        int damage = base_damage;
        for(unsigned int i = 0; i < modifiers.size(); i++) {
            const unsigned int RANDOM_MODIFIER_NUM = 5; //THIS PROBABLY SHOULDN'T BE HERE, IF NUMBER OF MODIFIER CHANGES THIS SHOULD CHANGE TOO, I'M SO LAZY
            if( i == RANDOM_MODIFIER_NUM ) damage = std::floor(damage * (100-mod) / 100);
            else damage = pokeRound(damage * modifiers[i]);
        }

        buffer.push_back(damage);
    }

    return buffer;
}

std::vector<float> Pokemon::getDamagePercentage(const Turn& theTurn) const {
    std::vector<int> damages = getDamageInt(theTurn);
    std::vector<float> buffer;
    for(unsigned int i = 0; i < damages.size(); i++) buffer.push_back(damages[i] / float(getCurrentHP()) * 100);

    return buffer;
}

void Pokemon::recursiveDamageCalculation(Pokemon theDefendingPokemon, std::vector<int>& theIntVector, std::vector<std::pair<Pokemon, Move>>& theVector, const unsigned int theHitNumber, std::vector<std::pair<Pokemon, Move>>::iterator& it) const {
    std::vector<std::pair<Pokemon, Move>> buffer_vector = theVector; //WE DO THIS BECAUSE LATER ON WE NEED TO MODIFY SOME OF THE TURN FOR DAMAGE CALCULATION PURPOSES AND IT'S BETTER TO MODIFY A COPY OF THE VECTOR
    std::vector<std::pair<Pokemon, Move>>::iterator buffer_it = it;

    if( it == theVector.begin() ) {
        theIntVector = theDefendingPokemon.getDamage(buffer_it->first, buffer_it->second);
    }

    else if( it == theVector.end() ) return;

    else {
        std::vector<int> buffer;
        for(unsigned int j = 0; j < theIntVector.size(); j++) {
            int new_hp = theDefendingPokemon.getCurrentHP() - theIntVector[j];
            if( new_hp < 0 ) new_hp = 0;
            if( new_hp > theDefendingPokemon.getBoostedStat(Stats::HP) ) new_hp = theDefendingPokemon.getStat(Stats::HP);
            theDefendingPokemon.setCurrentHPPercentage((new_hp/theDefendingPokemon.getBoostedStat(Stats::HP))*100);
            auto new_damages = theDefendingPokemon.getDamage(it->first, it->second);
            for( auto it2 = new_damages.begin(); it2 < new_damages.end(); it2++ ) buffer.push_back(*it2 + theIntVector[j]);
        }

        theIntVector = buffer;
    }

    //INFRA TURN MODIFIERS (these effects apply after each attack in a turn)
    if( buffer_it->second.isParentalBondMove() ) { //adding another move if it is a parental bond
        Pokemon parental_pokemon = buffer_it->first;
        Move parental_move = buffer_it->second;
        parental_move.setBasePower(parental_move.getBasePower()/4);
        parental_move.setParentalBondMove(false);
         it = theVector.insert(it+1, std::make_pair(parental_pokemon, parental_move));
         it--;
    }
    if( buffer_it->second.getMoveIndex() == Moves::Knock_Off && theDefendingPokemon.getItem().isRemovable() && !buffer_it->second.isZ() && !buffer_it->second.isParentalBondMove() ) theDefendingPokemon.setItem(Item(Items::None)); //setting the item as none after a knock off
    if( theDefendingPokemon.getItem().isReducingBerry() && theDefendingPokemon.getItem().getReducingBerryType() == buffer_it->second.getMoveType() && calculateTypeModifier(buffer_it->first, buffer_it->second) >= 2 ) theDefendingPokemon.setItem(Item(Items::None)); //setting the item as none if a reducing berry is consumed

    //END OF TURN MODIFIERS (these effects apply at the end of the turn
    //if the pokemon is not dead and some effects are in place we modify the damages
    if( std::distance(theVector.begin(), it) % theHitNumber == 0 ) {
        for( auto it_last = theIntVector.begin(); it_last < theIntVector.end(); it_last++ ) {
            if( *it_last < theDefendingPokemon.getBoostedStat(Stats::HP) ) {

                //RESTORING FOR GRASSY TERRAIN
                for( auto it_temp = theVector.begin(); it_temp < theVector.end(); it_temp++ ) {
                    if( it_temp->second.getTerrain() == Move::Terrain::GRASSY && theDefendingPokemon.isGrounded() ) {
                        *it_last = *it_last - (theDefendingPokemon.getBoostedStat(Stats::HP) / 16);
                        break; //grassy terrain just restores one time
                    }
                }

                //SOON SOME MORE
            }
        }
    }

    it++;
    recursiveDamageCalculation(theDefendingPokemon, theIntVector, theVector, theHitNumber, it);
}

std::vector<int> Pokemon::getDamageInt(const Turn& theTurn) const {
    std::vector<std::pair<Pokemon, Move>> buffer = theTurn.getMovesEffective();
    std::vector<std::pair<Pokemon, Move>>::iterator it = buffer.begin();
    std::vector<int> vec;

    recursiveDamageCalculation(*this, vec, buffer, theTurn.getHits(), it);
    return vec;
}

float Pokemon::getKOProbability(const Turn& theTurn) const {
    std::vector<float> damages = getDamagePercentage(theTurn);

    unsigned int ko_count = 0;
    for(auto it = damages.begin(); it < damages.end(); it++) if( *it >= 100 ) ko_count++;

    return (float(ko_count) / damages.size()) * 100;
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

DefenseResult Pokemon::resistMove(const std::vector<Turn>& theTurn, const std::vector<defense_modifier>& theDefModifiers) {
    //this would'nt be a valid calculation
    if( theTurn.size() != theDefModifiers.size() ) {
        DefenseResult temp;
        temp.hp_ev.push_back(-2);
        temp.def_ev.push_back(-2);
        temp.spdef_ev.push_back(-2);

        return temp;
    }

    //if there is no attack in the vector
    if( theTurn.empty() ) {
        DefenseResult temp;
        temp.hp_ev.push_back(-3);
        temp.def_ev.push_back(-3);
        temp.spdef_ev.push_back(-3);

        return temp;
    }

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

    auto results = resistMoveLoop(theTurn, theDefModifiers, simplified, simplified_type);
    //if no result has been found
    if( results.empty() && !abort_calculation ) {
        DefenseResult temp;
        temp.hp_ev.push_back(-1);
        temp.def_ev.push_back(-1);
        temp.spdef_ev.push_back(-1);

        return temp;
    }

    //if an abort has been requested
    else if( abort_calculation ) {
        DefenseResult temp;
        temp.hp_ev.push_back(-4);
        temp.def_ev.push_back(-4);
        temp.spdef_ev.push_back(-4);

        return temp;
    }

    DefenseResult returnable;

    for(unsigned int i = 0; i < RESULT_TYPE_NUM; i++) {
        const float WEIGHT = 2; //this is the weight used when calculating spreads more focused on hp (the bigger is weight the more they are focused on hp)
        float effective_weight;
        if( i == 0 ) effective_weight = 1;
        else effective_weight = WEIGHT * i;

        //finding all the minimum elements
        std::vector<unsigned int> sum_results;
        //creating the vector with the sums
        for( auto it = results.begin(); it < results.end(); it++ ) sum_results.push_back(std::get<0>(*it) + std::get<1>(*it) * effective_weight + std::get<2>(*it) * effective_weight );
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

        if( (i == 0) || (returnable.hp_ev[i-1] != std::get<0>(final_pair) || returnable.def_ev[i-1] != std::get<1>(final_pair) || returnable.spdef_ev[i-1] != std::get<2>(final_pair)  ) ) {
            returnable.hp_ev.push_back(std::get<0>(final_pair));
            returnable.def_ev.push_back(std::get<1>(final_pair));
            returnable.spdef_ev.push_back(std::get<2>(final_pair));
        }

        //if( !simplified ) break; //if we are not in a simplified loop (meaning we are calculating for attacks on different sides) there is really no need to move on
    }

    //now calculating all the damages
    for( unsigned int i = 0; i < returnable.hp_ev.size(); i++ ) { //we use hp.ev.size but we could really use any of the 3
        Pokemon buffer = *this;
        buffer.setEV(Stats::HP, returnable.hp_ev[0]);
        buffer.setEV(Stats::DEF, returnable.def_ev[0]);
        buffer.setEV(Stats::SPDEF, returnable.spdef_ev[0]);

        returnable.def_ko_prob.push_back(std::vector<float>());
        returnable.def_damage_perc.push_back(std::vector<std::vector<float>>());
        returnable.def_damage_int.push_back(std::vector<std::vector<int>>());
        for( auto it = 0; it < theTurn.size(); it++ ) {
            buffer.setModifier(Stats::HP, std::get<0>(theDefModifiers[it]));
            buffer.setModifier(Stats::DEF, std::get<1>(theDefModifiers[it]));
            buffer.setModifier(Stats::SPDEF, std::get<2>(theDefModifiers[it]));
            returnable.def_ko_prob[i].push_back(buffer.getKOProbability(theTurn[it]));
            returnable.def_damage_perc[i].push_back(buffer.getDamagePercentage(theTurn[it]));
            returnable.def_damage_int[i].push_back(buffer.getDamageInt(theTurn[it]));
        }
    }

    return returnable;
}

std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> Pokemon::resistMoveLoop(const std::vector<Turn>& theTurn, const std::vector<defense_modifier>& theDefModifiers, const bool isSimplified, const Move::Category simplifiedType) {
    const unsigned int MAX_EVS = 510;
    const unsigned int MAX_EVS_SINGLE_STAT = 252;
    const unsigned int ARRAY_SIZE = MAX_EVS_SINGLE_STAT + 1;

    unsigned int THREAD_NUM = std::thread::hardware_concurrency();
    if( THREAD_NUM == 0 ) THREAD_NUM = 1; //otherwise single thread machines would bug out
    std::vector<std::thread*> threads;

    Pokemon defender = *this;

    unsigned int assignable_evs = MAX_EVS - (defender.getEV(Stats::ATK) + defender.getEV(Stats::SPATK) + defender.getEV(Stats::SPE));
    defender.setAllEV(0, 0, 0, 0, 0, 0);

    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> results;

    std::vector<std::vector<float>> results_buffer;
    results_buffer.resize(theTurn.size());
    for(auto it = results_buffer.begin(); it < results_buffer.end(); it++) it->resize(ARRAY_SIZE*ARRAY_SIZE*ARRAY_SIZE);

    for(unsigned int hp_assigned = getEV(Stats::HP); hp_assigned < MAX_EVS_SINGLE_STAT + 1; hp_assigned = hp_assigned + calculateEVSNextStat(defender, Stats::HP, hp_assigned)) {
        defender.setEV(Stats::HP, hp_assigned);

        for(unsigned int spdef_assigned = getEV(Stats::SPDEF); spdef_assigned < MAX_EVS_SINGLE_STAT + 1; spdef_assigned = spdef_assigned + calculateEVSNextStat(defender, Stats::SPDEF, spdef_assigned)) {
            if( isSimplified && simplifiedType == Move::PHYSICAL && spdef_assigned > getEV(Stats::SPDEF) ) break;
                defender.setEV(Stats::SPDEF, spdef_assigned);

                for(unsigned int def_assigned = getEV(Stats::DEF); def_assigned < MAX_EVS_SINGLE_STAT + 1; def_assigned = def_assigned + calculateEVSNextStat(defender, Stats::DEF, def_assigned)) {
                    //qDebug() << QString::number(hp_assigned) + " " + QString::number(spdef_assigned) + " " + QString::number(def_assigned);
                    if( isSimplified && simplifiedType == Move::SPECIAL && def_assigned > getEV(Stats::DEF) ) break;
                    //if an abort has been requested we return an empty result
                    if( abort_calculation ) return std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>();

                    defender.setEV(Stats::DEF, def_assigned);

                    //if we are on a single-core architecture launching it in a separate thread just creates an useless overhead
                    if( THREAD_NUM < 2 ) resistMoveLoopThread(defender, theTurn, results, theDefModifiers, results_buffer, assignable_evs);

                    else {
                        threads.push_back(new std::thread(&Pokemon::resistMoveLoopThread, this, defender, std::cref(theTurn), std::ref(results), std::cref(theDefModifiers), std::ref(results_buffer), assignable_evs));
                        //if we spawned too much thread
                        if( threads.size() >= THREAD_NUM ) {
                            for( auto it = threads.begin(); it < threads.end(); it++ ) { (*it)->join(); delete *it; }
                            threads.clear();
                        }
                    }
                }
            }
        }

    //just in case some thread hasn't finished yet
    for( auto it = threads.begin(); it < threads.end(); it++ ) { (*it)->join(); delete *it; }
    threads.clear();

    //if an abort has been requested between the loop and the join operation we return an empty result
    if( abort_calculation ) return std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>();

    //if no result is found we search some rolls
    unsigned int roll_count = 0;
    const unsigned int MAX_ROLL = 70;
    const unsigned int ROLL_OFFSET = 1;
    std::vector<float> tolerances;
    tolerances.resize(theTurn.size(), 0);

    while( results.empty() && roll_count < (MAX_ROLL+1) * theTurn.size() && !abort_calculation/*((MAX_ROLL+1)*(pow(MAX_ROLL+1, theTurn.size()-1))) used for the alternative version of the algorithm*/ ) {
        //for( auto it = tolerances.begin(); it < tolerances.end(); it++ ) qDebug() << *it;

        unsigned int tolerance_index = roll_count / (MAX_ROLL+1);
        (*(tolerances.end()-1-tolerance_index)) += ROLL_OFFSET;

        /*these are two different algorithm that produce different result i like the first one more so i'm keeping it for now
        bool to_increment = true;
        for( auto it = tolerances.begin(); it < tolerances.end(); it++ ) {
            if( *it == MAX_ROLL ) {
                bool to_increment_local = true;
                for( auto it2 = it+1; it2 < tolerances.end(); it2++ ) if( *it2 != MAX_ROLL ) to_increment_local = false;
                if( to_increment_local && (*(tolerances.begin())) != MAX_ROLL ) {
                    (*(it-1)) += ROLL_OFFSET;
                    for(auto it3 = it; it3 < tolerances.end(); it3++) *it3 = 0;
                    to_increment = false;
                }
            }
        }

        if( to_increment ) tolerances.back()++;
        */

        for(unsigned int hp_assigned = getEV(Stats::HP); hp_assigned < MAX_EVS_SINGLE_STAT + 1; hp_assigned = hp_assigned + calculateEVSNextStat(defender, Stats::HP, hp_assigned)) {

            for(unsigned int spdef_assigned = getEV(Stats::SPDEF); spdef_assigned < MAX_EVS_SINGLE_STAT + 1; spdef_assigned = spdef_assigned + calculateEVSNextStat(defender, Stats::SPDEF, spdef_assigned)) {
                if( isSimplified && simplifiedType == Move::PHYSICAL && spdef_assigned > getEV(Stats::SPDEF) ) break;

                for(unsigned int def_assigned = getEV(Stats::DEF); def_assigned < MAX_EVS_SINGLE_STAT + 1; def_assigned = def_assigned + calculateEVSNextStat(defender, Stats::DEF, def_assigned)) {
                    //qDebug() << QString::number(roll_count) + " " + QString::number(hp_assigned) + " " + QString::number(spdef_assigned) + " " + QString::number(def_assigned);
                    //if an abort request is made
                    if( abort_calculation ) return std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>();
                    if( isSimplified && simplifiedType == Move::SPECIAL && def_assigned > getEV(Stats::DEF) ) break;

                    bool to_add = true;
                    for(unsigned int it = 0; it < theTurn.size() && to_add; it++ ) {
                        defender.setCurrentHPPercentage(std::get<0>(theDefModifiers[it]));
                        defender.setModifier(Stats::DEF, std::get<1>(theDefModifiers[it]));
                        defender.setModifier(Stats::SPDEF, std::get<2>(theDefModifiers[it]));
                        if( hp_assigned + def_assigned + spdef_assigned > assignable_evs ) to_add = false;
                        else if( results_buffer[it][hp_assigned +  def_assigned * ARRAY_SIZE + spdef_assigned * ARRAY_SIZE * ARRAY_SIZE] > (0 + tolerances[it]) ) to_add = false;
                    }

                    if( to_add ) results.push_back(std::make_tuple(hp_assigned, def_assigned, spdef_assigned));
                }
            }

        }

        roll_count++;
    }


    return results;
}

void Pokemon::resistMoveLoopThread(Pokemon theDefender, const std::vector<Turn>& theTurn, std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>& theResult, const std::vector<defense_modifier>& theDefModifiers, std::vector<std::vector<float>>& theResultBuffer, const unsigned int theAssignableEVS) {
    const unsigned int ARRAY_SIZE = 253; //this should be passed as an argument by the main thread but ehi...am i lazy or not?

    bool to_add = true;
    for(unsigned int it = 0; it < theTurn.size(); it++ ) {
        theDefender.setCurrentHPPercentage(std::get<0>(theDefModifiers[it]));
        theDefender.setModifier(Stats::DEF, std::get<1>(theDefModifiers[it]));
        theDefender.setModifier(Stats::SPDEF, std::get<2>(theDefModifiers[it]));
        float ko_prob;
        if( theDefender.getEV(Stats::HP) + theDefender.getEV(Stats::DEF) + theDefender.getEV(Stats::SPDEF) > theAssignableEVS ) to_add = false;
        else if( (ko_prob = theDefender.getKOProbability(theTurn[it])) > 0 ) {
            buffer_mutex.lock();
            theResultBuffer[it][theDefender.getEV(Stats::HP) + theDefender.getEV(Stats::DEF) * ARRAY_SIZE + theDefender.getEV(Stats::SPDEF) * ARRAY_SIZE * ARRAY_SIZE] = ko_prob;
            buffer_mutex.unlock();
            to_add = false;
        }

        else {
            buffer_mutex.lock();
            theResultBuffer[it][theDefender.getEV(Stats::HP) + theDefender.getEV(Stats::DEF) * ARRAY_SIZE + theDefender.getEV(Stats::SPDEF) * ARRAY_SIZE * ARRAY_SIZE] = ko_prob;
            buffer_mutex.unlock();
        }
    }

    if( to_add ) {
        result_mutex.lock();
        theResult.push_back(std::make_tuple(theDefender.getEV(Stats::HP), theDefender.getEV(Stats::DEF), theDefender.getEV(Stats::SPDEF)));
        result_mutex.unlock();
    }
}

int Pokemon::pokeRound(const float theValue) const {
    if( fmod(theValue, 1) > 0.5 ) return ceil(theValue);
    else return floor(theValue);
}

AttackResult Pokemon::koMove(const std::vector<Turn>& theTurn, const std::vector<Pokemon>& theDefendingPokemon, const std::vector<attack_modifier>& theAtkModifier) {
    //this would'nt be a valid calculation
    if( theTurn.size() != theAtkModifier.size() || theTurn.size() != theDefendingPokemon.size() ) {
        AttackResult temp;
        temp.atk_ev.push_back(-2);
        temp.spatk_ev.push_back(-2);

        return temp;
    }

    //if there is no attack in the vector
    if( theTurn.empty() ) {
        AttackResult temp;
        temp.atk_ev.push_back(-3);
        temp.spatk_ev.push_back(-3);

        return temp;
    }

    //algorithm here
    const unsigned int MAX_EVS = 510;
    const unsigned int MAX_EVS_SINGLE_STAT = 252;
    const unsigned int ARRAY_SIZE = MAX_EVS_SINGLE_STAT + 1;

    Pokemon attacker = *this;

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

    unsigned int assignable_evs = MAX_EVS - (attacker.getEV(Stats::HP) + attacker.getEV(Stats::DEF) + attacker.getEV(Stats::SPDEF) + attacker.getEV(Stats::SPE));
    attacker.setAllEV(0, 0, 0, 0, 0, 0);

    std::vector<std::pair<int, int>> results;

    std::vector<std::vector<float>> results_buffer;
    results_buffer.resize(theTurn.size());
    for(auto it = results_buffer.begin(); it < results_buffer.end(); it++) it->resize(ARRAY_SIZE*ARRAY_SIZE);

    for(unsigned int spatk_assigned = getEV(Stats::SPATK); spatk_assigned < MAX_EVS_SINGLE_STAT + 1; spatk_assigned = spatk_assigned + calculateEVSNextStat(attacker, Stats::SPATK, spatk_assigned)) {
        if( simplified && simplified_type == Move::PHYSICAL && spatk_assigned > getEV(Stats::SPATK) ) break;
        attacker.setEV(Stats::SPATK, spatk_assigned);

        for(unsigned int atk_assigned = getEV(Stats::ATK); atk_assigned < MAX_EVS_SINGLE_STAT + 1; atk_assigned = atk_assigned + calculateEVSNextStat(attacker, Stats::ATK, atk_assigned)) {
            if( simplified && simplified_type == Move::SPECIAL && atk_assigned > getEV(Stats::ATK) ) break;
            //if an abort has been requested we return an empty result
            if( abort_calculation ) { AttackResult temp; temp.atk_ev.push_back(-4); temp.spatk_ev.push_back(-4); return temp; }

            attacker.setEV(Stats::ATK, atk_assigned);

            const unsigned int ARRAY_SIZE = 253;

            bool to_add = true;
            for(unsigned int it = 0; it < theTurn.size(); it++ ) {
                attacker.setModifier(Stats::ATK, theAtkModifier[it].first);
                attacker.setModifier(Stats::SPATK, theAtkModifier[it].second);

                //reversing because of the offensive nature of the calc
                Turn temp_turn;
                temp_turn.addMove(attacker, theTurn[it].getMoves()[0].second);
                temp_turn.setHits(theTurn[it].getHits());

                float ko_prob;
                if( attacker.getEV(Stats::ATK) + attacker.getEV(Stats::SPATK) > assignable_evs ) to_add = false;
                else if( (ko_prob = theDefendingPokemon[it].getKOProbability(temp_turn)) < 100 ) { results_buffer[it][attacker.getEV(Stats::ATK) + attacker.getEV(Stats::SPATK) * ARRAY_SIZE] = ko_prob; to_add = false; }

                else results_buffer[it][attacker.getEV(Stats::ATK) + attacker.getEV(Stats::SPATK) * ARRAY_SIZE] = ko_prob;
            }

            if( to_add ) results.push_back(std::make_pair(attacker.getEV(Stats::ATK), attacker.getEV(Stats::SPATK)));
        }
    }

    //if no result is found we search some rolls
    unsigned int roll_count = 0;
    const unsigned int MAX_ROLL = 70;
    const unsigned int ROLL_OFFSET = 1;
    std::vector<float> tolerances;
    tolerances.resize(theTurn.size(), 0);

    while( results.empty() && roll_count < (MAX_ROLL+1) * theTurn.size() && !abort_calculation/*((MAX_ROLL+1)*(pow(MAX_ROLL+1, theTurn.size()-1))) used for the alternative version of the algorithm*/ ) {
        //for( auto it = tolerances.begin(); it < tolerances.end(); it++ ) qDebug() << *it;

        unsigned int tolerance_index = roll_count / (MAX_ROLL+1);
        (*(tolerances.end()-1-tolerance_index)) += ROLL_OFFSET;

        /*these are two different algorithm that produce different result i like the first one more so i'm keeping it for now
        bool to_increment = true;
        for( auto it = tolerances.begin(); it < tolerances.end(); it++ ) {
            if( *it == MAX_ROLL ) {
                bool to_increment_local = true;
                for( auto it2 = it+1; it2 < tolerances.end(); it2++ ) if( *it2 != MAX_ROLL ) to_increment_local = false;
                if( to_increment_local && (*(tolerances.begin())) != MAX_ROLL ) {
                    (*(it-1)) += ROLL_OFFSET;
                    for(auto it3 = it; it3 < tolerances.end(); it3++) *it3 = 0;
                    to_increment = false;
                }
            }
        }

        if( to_increment ) tolerances.back()++;
        */

        for(unsigned int spatk_assigned = 0; spatk_assigned < MAX_EVS_SINGLE_STAT + 1; spatk_assigned = spatk_assigned + calculateEVSNextStat(attacker, Stats::SPATK, spatk_assigned)) {
            if( simplified && simplified_type == Move::PHYSICAL && spatk_assigned > getEV(Stats::SPATK) ) break;

            for(unsigned int atk_assigned = 0; atk_assigned < MAX_EVS_SINGLE_STAT + 1; atk_assigned = atk_assigned + calculateEVSNextStat(attacker, Stats::ATK, atk_assigned)) {
                //qDebug() << QString::number(roll_count) + " " + QString::number(hp_assigned) + " " + QString::number(spdef_assigned) + " " + QString::number(def_assigned);
                //if an abort request is made
                if( abort_calculation ) { AttackResult temp; temp.atk_ev.push_back(-4); temp.spatk_ev.push_back(-4); return temp; }
                if( simplified && simplified_type == Move::SPECIAL && atk_assigned > getEV(Stats::ATK) ) break;

                bool to_add = true;
                for(unsigned int it = 0; it < theTurn.size() && to_add; it++ ) {
                    if( spatk_assigned + atk_assigned > assignable_evs ) to_add = false;
                    else if( results_buffer[it][atk_assigned + spatk_assigned * ARRAY_SIZE] < (100 - tolerances[it]) ) to_add = false;
                }

                if( to_add ) results.push_back(std::make_pair(atk_assigned, spatk_assigned));
            }
        }

    roll_count++;
    }
    //

    //if no result has been found
    if( results.empty() && !abort_calculation ) {
        AttackResult temp;
        temp.atk_ev.push_back(-1);
        temp.spatk_ev.push_back(-1);

        return temp;
    }

    //if an abort has been requested
    else if( abort_calculation ) {
        AttackResult temp;
        temp.atk_ev.push_back(-4);
        temp.spatk_ev.push_back(-4);

        return temp;
    }

    //finding all the minimum elements
    std::vector<unsigned int> sum_results;
    //creating the vector with the sums
    for( auto it = results.begin(); it < results.end(); it++ ) sum_results.push_back(it->first + it->second);
    //finding the minimum sums
    auto min_index = std::min_element(sum_results.begin(), sum_results.end());

    AttackResult final_result;
    final_result.atk_ev.push_back(results[std::distance(sum_results.begin(), min_index)].first);
    final_result.spatk_ev.push_back(results[std::distance(sum_results.begin(), min_index)].second);

    //now calculating all the damages
    Pokemon buffer = *this;
    buffer.setEV(Stats::ATK, final_result.atk_ev[0]);
    buffer.setEV(Stats::SPATK, final_result.spatk_ev[0]);

    final_result.atk_ko_prob.push_back(std::vector<float>());
    final_result.atk_damage_perc.push_back(std::vector<std::vector<float>>());
    final_result.atk_damage_int.push_back(std::vector<std::vector<int>>());
    for( auto it = 0; it < theTurn.size(); it++ ) {
        //doing this switch because the Turn is used in an offensive sense
        buffer.setModifier(Stats::ATK, theAtkModifier[it].first);
        buffer.setModifier(Stats::SPATK, theAtkModifier[it].second);

        Turn temp_turn;
        temp_turn.addMove(buffer, theTurn[it].getMoves()[0].second);
        temp_turn.setHits(theTurn[it].getHits());

        final_result.atk_ko_prob[0].push_back(theDefendingPokemon[it].getKOProbability(temp_turn));
        final_result.atk_damage_perc[0].push_back(theDefendingPokemon[it].getDamagePercentage(temp_turn));
        final_result.atk_damage_int[0].push_back(theDefendingPokemon[it].getDamageInt(temp_turn));
    }

    return final_result;
}

std::pair<DefenseResult, AttackResult> Pokemon::calculateEVSDistrisbution(const EVCalculationInput& theInput) {
    //we do this calculation on a copy of *this
    Pokemon buffer = *this;

    if( theInput.priority == PRIORITY_DEFENSE ) {
        auto def_result = resistMove(theInput.def_turn, theInput.def_modifier);
        //saving these variable because we change them and we need to restore them back later
        unsigned int tamp_hp = getEV(Stats::HP);
        unsigned int tamp_def = getEV(Stats::DEF);
        unsigned int tamp_spdef = getEV(Stats::SPDEF);
        AttackResult atk_result;
        for(unsigned int i = 0; i < def_result.hp_ev.size(); i++) { //we use hp.ev but really could use any of the 3
            setEV(Stats::HP, def_result.hp_ev[i]);
            setEV(Stats::DEF, def_result.def_ev[i]);
            setEV(Stats::SPDEF, def_result.spdef_ev[i]);

            auto atk_buffer = koMove(theInput.atk_turn, theInput.defending_pokemon, theInput.atk_modifier);

            if( atk_buffer.isEmpty() && i > 0 ) { //if the alternative spreads are unsuitable because they don't match for the offensive side we erase them
                def_result.hp_ev.erase(def_result.hp_ev.begin()+i);
                def_result.def_ev.erase(def_result.def_ev.begin()+i);
                def_result.spdef_ev.erase(def_result.spdef_ev.begin()+i);
                def_result.def_ko_prob.erase(def_result.def_ko_prob.begin()+i);
                def_result.def_damage_int.erase(def_result.def_damage_int.begin()+i);
                def_result.def_damage_perc.erase(def_result.def_damage_perc.begin()+i);
                i--;
            }

            else {
                atk_result.atk_ev.push_back(atk_buffer.atk_ev[0]);
                atk_result.spatk_ev.push_back(atk_buffer.spatk_ev[0]);
                if( !atk_buffer.atk_ko_prob.empty() ) atk_result.atk_ko_prob.push_back(atk_buffer.atk_ko_prob[0]); //because in case of error the function returns empty vectors
                if( !atk_buffer.atk_damage_int.empty() ) atk_result.atk_damage_int.push_back(atk_buffer.atk_damage_int[0]);
                if( !atk_buffer.atk_damage_perc.empty() ) atk_result.atk_damage_perc.push_back(atk_buffer.atk_damage_perc[0]);
            }

        }

        //restoring them
        setEV(Stats::HP, tamp_hp);
        setEV(Stats::DEF, tamp_def);
        setEV(Stats::SPDEF, tamp_spdef);

        return std::make_pair(def_result, atk_result);
    }

    else {
        auto atk_result = koMove(theInput.atk_turn, theInput.defending_pokemon, theInput.atk_modifier);
        //same as before, saving them because we need to restore them
        unsigned int tamp_atk = getEV(Stats::ATK);
        unsigned int tamp_spatk = getEV(Stats::SPATK);

        setEV(Stats::ATK, atk_result.atk_ev[0]);
        setEV(Stats::SPATK, atk_result.spatk_ev[0]);

        auto def_result = resistMove(theInput.def_turn, theInput.def_modifier);

        //if the defense result returned more than 1 suitable spread we fill the atk result with the same result just for coherence
        if( def_result.hp_ev.size() > 1 ) { //honestly we could check any of them
            for( unsigned int i = 1; i < def_result.hp_ev.size(); i++ ) {
                atk_result.atk_ev.push_back(atk_result.atk_ev[i-1]);
                atk_result.spatk_ev.push_back(atk_result.spatk_ev[i-1]);
                atk_result.atk_ko_prob.push_back(atk_result.atk_ko_prob[i-1]);
                atk_result.atk_damage_int.push_back(atk_result.atk_damage_int[i-1]);
                atk_result.atk_damage_perc.push_back(atk_result.atk_damage_perc[i-1]);
            }
        }

        setEV(Stats::ATK, tamp_atk);
        setEV(Stats::SPATK, tamp_spatk);
        return std::make_pair(def_result, atk_result);
    }
}
