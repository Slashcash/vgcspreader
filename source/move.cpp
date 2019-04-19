#include "move.hpp"
#include "pokemon.hpp"

Move::Move(const Moves theMoveIndex) {
    index = theMoveIndex;

    base_power = (unsigned int)Pokemon::db.getMoveBasePower(theMoveIndex);
    z_base_power = (unsigned int)Pokemon::db.getMoveBasePowerZ(theMoveIndex);
    if( Pokemon::db.isSignatureZ(theMoveIndex) ) is_signature_z = true;
    else is_signature_z = false;
    is_z = false;
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
    parental_bond_move = false;
}
