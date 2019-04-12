#ifndef DEFENSERESULT_HPP
#define DEFENSERESULT_HPP

//this class is really simple and is mainly a getter/setter collection so don't bother about it

#include <vector>

class DefenseResult {
    public:
        int hp_ev;
        int def_ev;
        int spdef_ev;
        std::vector<float> def_ko_prob;
        std::vector<std::vector<float>> def_damage_perc;
        std::vector<std::vector<int>> def_damage_int;
};

#endif // DEFENSERESULT_HPP
