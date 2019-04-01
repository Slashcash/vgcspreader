#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include <cstdint>
#include <string>

#include "modifier.hpp"
#include "pokemon.hpp"
#include "moves.hpp"

class Move {
    public:
        enum Category {
            PHYSICAL,
            SPECIAL
        };

        enum Target {
            SINGLE,
            DOUBLE
        };

        enum Weather {
            WEATHER_NONE,
            SUN,
            RAIN,
            HARSH_SUNSHINE,
            HEAVY_RAIN,
            STRONG_WINDS
        };

        enum Terrain {
            TERRAIN_NONE,
            GRASSY,
            ELECTRIC,
            PSYCHIC,
            MISTY
        };

    private:
        Moves index;
        unsigned int base_power;
        unsigned int z_base_power;
        bool is_signature_z;
        bool is_z;
        Category move_category;
        Type move_type;
        bool is_spread;
        bool is_crit;
        Target target;
        Weather weather;
        Terrain terrain;
        bool dark_aura;
        bool fairy_aura;

        Modifier modifier;

    public:
        Move() = delete;
        Move(const Moves theMoveIndex);

        unsigned int getBasePower() const { return base_power; }
        unsigned int getZBasePower() const { return z_base_power; }
        bool isSignatureZ() const { return is_signature_z; }
        bool isZ() const { return is_z; }
        Category getMoveCategory() const { return move_category; }
        Type getMoveType() const { return move_type; }
        bool isSpread() const { return is_spread; }
        Target getTarget() const { return target; }
        Modifier getModifier() const { return modifier; }
        Weather getWeather() const { return weather; }
        Terrain getTerrain() const { return terrain; }
        bool isCrit() const { return is_crit; }
        bool isFairyAura() const { return fairy_aura; }
        bool isDarkAura() const { return dark_aura; }
        Moves getMoveIndex() const { return index; }

        void setBasePower(const unsigned int theBasePower) { base_power = theBasePower; }
        void setZ(const bool isZ) { is_z = isZ; }
        void setMoveCategory(const Category& theMoveCategory) { move_category = theMoveCategory; }
        void setMoveType(const Type& theType) { move_type = theType; }
        void setSpread(const bool isSpread) { is_spread = isSpread; }
        Modifier& setModifier() { return modifier; }
        void setTarget(const Target& theTarget) { target = theTarget; }
        void setWeather(const Weather& theWeather) { weather = theWeather; }
        void setTerrain(const Terrain& theTerrain) { terrain = theTerrain; }
        void setCrit(const bool isCrit) { is_crit = isCrit; }
        void setDarkAura(const bool isDarkAura) { dark_aura = isDarkAura; }
        void setFairyAura(const bool isFairyAura) { fairy_aura = isFairyAura; }
};

#endif
