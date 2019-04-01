#ifndef _POKEMON_HPP_
#define _POKEMON_HPP_

#include <string>
#include <array>
#include <vector>
#include <tuple>
#include <math.h>
#include <cmath>
#include <mutex>

#include "stats.hpp"
#include "modifier.hpp"
#include "pokemondb.hpp"
#include "types.hpp"
#include "abilities.hpp"
#include "item.hpp"

class Move;
class Turn;

typedef std::tuple<float, int16_t, int16_t> defense_modifier;

enum Status {
    NO_STATUS,
    BURNED,
    POISONED,
    PARALYZED
};

class Pokemon {
    private:
        static const float type_matrix[18][18]; //C STYLE FOR CONVENIENCE

        unsigned int pokedex_number;
        Stats stats;
        std::vector<std::array<uint8_t, 6>> base;
        std::array<uint16_t, 6> total;
        std::array<uint16_t, 6> boosted;
        std::vector<std::array<Ability, 3>> possible_abilities;
        Ability ability;
        Item item;
        float current_hp_percentage;
        std::vector<std::array<Type, 2>> types;
        Status status;
        bool grounded;
        unsigned int formes_number;
        unsigned int form;
        static std::mutex buffer_mutex;
        static std::mutex result_mutex;

        void calculateTotal();
        std::vector<int> getDamage(const Pokemon& theAttacker, Move theMove) const;
        float calculateWeatherModifier(const Move& theMove) const;
        float calculateTerrainModifier(const Pokemon& theAttacker, const Move& theMove) const;
        float calculateStabModifier(const Pokemon& theAttacker, const Move& theMove) const;
        float calculateTargetModifier(const Move& theMove) const;
        float calculateCritModifier(const Move& theMove) const;
        float calculateBurnModifier(const Pokemon& theAttacker, const Move& theMove) const;
        float calculateOtherModifier(const Pokemon& theAttacker, const Move& theMove) const;
        float calculateTypeModifier(const Pokemon& theAttacker, const Move& theMove) const;

        int pokeRound(const float theValue) const;
        uint16_t calculateAttackInMove(const Pokemon& theAttacker, const Move& theMove) const;
        uint16_t calculateDefenseInMove(const Move& theMove) const;
        unsigned int calculateMoveBasePowerInAttack(const Pokemon& theAttacker, const Move& theMove) const;

        void recursiveDamageCalculation(Pokemon theDefendingPokemon, std::vector<int>& theIntVector, std::vector<std::pair<Pokemon, Move>>& theVector, const unsigned int theHitNumber, std::vector<std::pair<Pokemon, Move>>::iterator& it) const;
        uint8_t calculateEVSNextStat(Pokemon thePokemon, const Stats::Stat& theStat, const unsigned int theStartingEVS) const;
        std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> resistMoveLoop(const std::vector<Turn>& theTurn, const std::vector<defense_modifier>& theDefModifiers);
        void resistMoveLoopThread(Pokemon theDefender, const std::vector<Turn>& theTurn, std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>& theResult, const std::vector<defense_modifier>& theDefModifiers, std::vector<std::vector<float>>& theResultBuffer, const unsigned int theAssignableEVS); //IS THIS A MONSTER? YES IT IS, FORGIVE ME

    public:
        static PokemonDB db;

        Pokemon() = delete;
        Pokemon(const unsigned int thePokedexNumber, const Stats& theStats = Stats());

        void setLevel(const uint8_t theLevel) { stats.setLevel(theLevel); calculateTotal(); }
        void setNature(const Stats::Nature& theNature) { stats.setNature(theNature); calculateTotal(); }
        void setAllIV(const uint8_t hp, const uint8_t atk, const uint8_t def, const uint8_t spatk, const uint8_t spdef, const uint8_t spe) { stats.setAllIV(hp, atk, def, spatk, spdef, spe); calculateTotal(); }
        void setIV(const Stats::Stat theStat, const uint8_t theValue) { stats.setIV(theStat, theValue); calculateTotal(); }
        void setAllEV(const uint8_t hp, const uint8_t atk, const uint8_t def, const uint8_t spatk, const uint8_t spdef, const uint8_t spe) { stats.setAllEV(hp, atk, def, spatk, spdef, spe); calculateTotal(); }
        void setEV(const Stats::Stat theStat, const uint8_t theValue) { stats.setEV(theStat, theValue); calculateTotal(); }
        void setAllModifier(const int16_t atk, const int16_t def, const int16_t spatk, const int16_t spdef, const int16_t spe) { stats.setAllModifier(atk, def, spatk, spdef, spe); calculateTotal(); }
        void setModifier(const Stats::Stat theStat, const int16_t theValue) { stats.setModifier(theStat, theValue); calculateTotal(); }
        void setStatus(const Status theStatus) { status = theStatus; }
        void setCurrentHPPercentage(const float theHPPercentage) { if( theHPPercentage <= 100 ) current_hp_percentage = theHPPercentage; else current_hp_percentage = 100;  }
        void setPossibleAbilities(const std::vector<std::array<Ability, 3>> thePossibleAbilities) { possible_abilities = thePossibleAbilities; }
        void setAbility(const Ability theAbility) { ability = theAbility; }
        void setItem(const Item theItem) { item = theItem; calculateTotal(); }
        void setType(const unsigned int theIndex, const Type& theType) { types[form][theIndex] = theType; }
        void setGrounded(const bool theValue) { grounded = theValue; }
        void setForm(const unsigned int theForm) { if( theForm < formes_number ) { form = theForm; ability = possible_abilities[form][0]; calculateTotal(); } }

        unsigned int getPokedexNumber() const { return pokedex_number; }
        uint8_t getLevel() const { return stats.getLevel(); }
        uint8_t getIV(const Stats::Stat& theStat) const { return stats.getIV(theStat); }
        uint8_t getEV(const Stats::Stat& theStat) const { return stats.getEV(theStat); }
        uint8_t getBaseStat(const Stats::Stat& theStat) const { return base[form][theStat]; }
        uint16_t getStat(const Stats::Stat& theStat) const { return total[theStat]; }
        uint16_t getBoostedStat(const Stats::Stat& theStat) const { return boosted[theStat]; }
        Stats::Nature getNature() const { return stats.getNature(); }
        Status getStatus() const { return status; }
        std::vector<std::array<Type, 2>> getTypes() const { return types; }
        float getCurrentHPPercentage() const { return current_hp_percentage; }
        uint16_t getCurrentHP() const { return ( boosted[Stats::HP] * current_hp_percentage ) / 100; }
        std::vector<std::array<Ability, 3>> getPossibleAbilities() const { return possible_abilities; }
        Ability getAbility() const { return ability; }
        int16_t getModifier(Stats::Stat theStat) const { return stats.getModifier(theStat); }
        Item getItem() const { return item; }
        bool isGrounded() const { return grounded; }
        unsigned int getForm() const { return form; }
        unsigned int getFormesNumber() const { return formes_number; }

        std::vector<int> getDamageInt(const Turn& theTurn) const;
        std::vector<float> getDamagePercentage(const Turn& theTurn) const;
        float getKOProbability(const Turn& theTurn) const;
        std::tuple<int, int, int> resistMove(const std::vector<Turn>& theTurn, const std::vector<defense_modifier>& theDefModifiers, std::vector<float>& theKoProbability);
        int outspeedPokemon(const std::vector<Pokemon>& theVector);
};

#endif
