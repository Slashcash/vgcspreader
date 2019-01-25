#include "move.hpp"

Move::Move(const Moves theMoveIndex) {
    index = theMoveIndex;

    base_power = (unsigned int)Pokemon::db.getMoveBasePower(theMoveIndex);
    move_category = (Category)Pokemon::db.getMoveCategory(theMoveIndex);
    move_type = (Type)Pokemon::db.getMoveType(theMoveIndex);
    is_spread = Pokemon::db.isMoveSpread(theMoveIndex);
    if( is_spread ) target = DOUBLE;
    else target = SINGLE;
    weather = WEATHER_NONE;
    terrain = TERRAIN_NONE;
    is_crit = false;
    dark_aura = false;
    fairy_aura = false;
}
