#include "item.hpp"

#include "items.hpp"
#include "pokemon.hpp"

Item::Item() {
    setIndex(Items::None);
}

Item::Item(const unsigned int theIndex) {
    setIndex(theIndex);
}

void Item::setIndex(const unsigned int theIndex) {
    index = theIndex;
    is_removable = Pokemon::db.isItemRemovable(index);
    is_reducing_berry = Pokemon::db.isItemReducingBerry(index);
    reducing_berry_type = (Type)Pokemon::db.getReducingBerryType(index);
}
