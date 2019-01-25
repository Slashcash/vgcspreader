#ifndef _MODIFIER_HPP_
#define _MODIFIER_HPP_

#include <array>

class Modifier {
    private:
        friend class Pokemon;

        enum Mod {
            TARGET,
            WEATHER,
            BADGE,
            CRITICAL,
            RANDOM,
            STAB,
            TYPE,
            BURN,
            OTHER,
            MOD_NUM
        };

        std::array<float, MOD_NUM> modifiers;

        void setRandomModifier(const float theModifier) { modifiers[RANDOM] = theModifier; }
        void setWeatherModifier(const float theModifier) { modifiers[WEATHER] = theModifier; }
        void setStabModifier(const float theModifier) { modifiers[STAB] = theModifier; }
        void setTargetModifier(const float theModifier) { modifiers[TARGET] = theModifier; }
        void setCritModifier(const float theModifier) { modifiers[CRITICAL] = theModifier; }
        void setBurnModifier(const float theModifier) { modifiers[BURN] = theModifier; }
        void setTypeModifier(const float theModifier) { modifiers[TYPE] = theModifier; }
        void setOtherModifier(const float theModifier) { modifiers[OTHER] = theModifier; }

        float getRandomModifier() const { return modifiers[RANDOM]; }
        float getWeatherModifier() const { return modifiers[WEATHER]; }
        float getStabModifier() const { return modifiers[STAB]; }
        float getTargetModifier() const { return modifiers[TARGET]; }
        float getCritModifier() const { return modifiers[CRITICAL]; }
        float getBurnModifier() const { return modifiers[BURN]; }
        float getTypeModifier() const { return modifiers[TYPE]; }
        float getOtherModifier() const { return modifiers[OTHER]; }

        std::array<float, MOD_NUM> getModifiers() const { return modifiers; }

    public:
        Modifier();
};

#endif
