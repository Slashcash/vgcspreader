#ifndef _STATS_HPP_
#define _STATS_HPP_

#include <cstdint>
#include <array>

class Stats {
    public:
        enum Stat {
            HP = 0,
            ATK,
            DEF,
            SPATK,
            SPDEF,
            SPE,
            STAT_NUM
        };

        enum Nature {
            HARDY, LONELY, BRAVE, ADAMANT, NAUGHTY,
            BOLD, DOCILE, RELAXED, IMPISH, LAX,
            TIMID, HASTY, SERIOUS, JOLLY, NAIVE,
            MODEST, MILD, QUIET, BASHFUL, RASH,
            CALM, GENTLE, SASSY, CAREFUL, QUIRK,
            NATURE_NUM
        };

    private:
        uint8_t level;
        std::array<uint8_t, STAT_NUM> iv;
        std::array<uint8_t, STAT_NUM> ev;
        std::array<int16_t, STAT_NUM-1> modifier;
        Nature nature;

    public:
        Stats();

        uint8_t getLevel() const { return level; }
        uint8_t getIV(const Stat theStat) const { return iv[theStat]; }
        uint8_t getEV(const Stat theStat) const { return ev[theStat]; }
        uint16_t getModifier(const Stat theStat) const;
        Nature getNature() const { return nature; }

        void setLevel(const uint8_t theLevel) { level = theLevel; }
        void setIV(const Stat theStat, const uint8_t theValue) { iv[theStat] = theValue; }
        void setAllIV(const uint8_t hp, const uint8_t atk, const uint8_t def, const uint8_t spatk, const uint8_t spdef, const uint8_t spe);
        void setEV(const Stat theStat, const uint8_t theValue) { ev[theStat] = theValue; }
        void setAllEV(const uint8_t hp, const uint8_t atk, const uint8_t def, const uint8_t spatk, const uint8_t spdef, const uint8_t spe);
        void setModifier(const Stat theStat, const int16_t theModifier);
        void setAllModifier(const int16_t atk, const int16_t def, const int16_t spatk, const int16_t spdef, const int16_t spe);
        void setNature(const Nature theNature) { nature = theNature; }
};

#endif // _STATS_HPP_
