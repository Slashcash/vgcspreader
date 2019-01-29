#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "items.hpp"

class Item {
    private:
        unsigned int index;
        bool is_removable;

     public:
        Item();
        Item(const unsigned int theIndex);

        bool isRemovable() const { return is_removable; }
        unsigned int getIndex() { return index; }

        void setIndex(const unsigned int theIndex);

        bool operator==(const Items& theItem) const { return index == theItem; }
};

#endif // ITEM_HPP
