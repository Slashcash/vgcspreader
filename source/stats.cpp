#include "stats.hpp"

Stats::Stats() {
    //set default level
    level = 50;

    //set default ivs
    for(unsigned int i = 0; i < iv.size(); i++) iv[i] = 31;

    //set default evs
    for(unsigned int i = 0; i < ev.size(); i++) ev[i] = 0;

    //set default modifier
    for(unsigned int i = 0; i < modifier.size(); i++) modifier[i] = 0;

    //set default nature
    nature = HARDY;
}

void Stats::setAllIV(const uint8_t hp, const uint8_t atk, const uint8_t def, const uint8_t spatk, const uint8_t spdef, const uint8_t spe) {
    setIV(HP, hp);
    setIV(ATK, atk);
    setIV(DEF, def);
    setIV(SPATK, spatk);
    setIV(SPDEF, spdef);
    setIV(SPE, spe);
}

void Stats::setAllEV(const uint8_t hp, const uint8_t atk, const uint8_t def, const uint8_t spatk, const uint8_t spdef, const uint8_t spe) {
    setEV(HP, hp);
    setEV(ATK, atk);
    setEV(DEF, def);
    setEV(SPATK, spatk);
    setEV(SPDEF, spdef);
    setEV(SPE, spe);
}

void Stats::setModifier(const Stat theStat, const int16_t theModifier) {
    if( theModifier >= -6 && theModifier <= 6 && theStat != HP ) {
        unsigned int temp_stat = theStat;
        temp_stat--;
        modifier[temp_stat] = theModifier;
    }
}

uint16_t Stats::getModifier(const Stat theStat) const {
    if( theStat == Stats::HP ) return 0;

    unsigned int temp_stat = theStat;
    temp_stat--;
    return modifier[temp_stat];
}

void Stats::setAllModifier(const int16_t atk, const int16_t def, const int16_t spatk, const int16_t spdef, const int16_t spe) {
    setModifier(ATK, atk);
    setModifier(DEF, def);
    setModifier(SPATK, spatk);
    setModifier(SPDEF, spdef);
    setModifier(SPE, spe);
}
