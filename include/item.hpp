#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "items.hpp"
#include "types.hpp"

class Item {
    private:
        unsigned int index;
        bool is_removable;
        bool is_reducing_berry;
        Type reducing_berry_type;

     public:
        Item();
        Item(const unsigned int theIndex);

        bool isRemovable() const { return is_removable; }
        bool isReducingBerry() const { return is_reducing_berry; }
        Type getReducingBerryType() const { return reducing_berry_type; }
        unsigned int getIndex() { return index; }

        void setIndex(const unsigned int theIndex);

        bool operator==(const Items& theItem) const { return index == theItem; }
};

#endif // ITEM_HPP
