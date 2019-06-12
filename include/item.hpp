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
        bool is_restoring_berry;
        float restoring_activation;
        float restoring_percentage;

     public:
        Item();
        Item(const unsigned int theIndex);

        bool isRemovable() const { return is_removable; }
        bool isReducingBerry() const { return is_reducing_berry; }
        Type getReducingBerryType() const { return reducing_berry_type; }
        bool isRestoringBerry() const { return is_restoring_berry; }
        float getRestoringActivation() const { return restoring_activation; }
        float getRestoringPercentage() const { return restoring_percentage; }

        unsigned int getIndex() { return index; }

        void setIndex(const unsigned int theIndex);

        bool operator==(const Items& theItem) const { return index == theItem; }
};

#endif // ITEM_HPP
