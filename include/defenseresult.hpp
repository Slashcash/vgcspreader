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

        bool isValid() const { return (hp_ev != -2 && def_ev != -2 && spdef_ev != -2); }
        bool isEmpty() const { return (hp_ev == -1 && def_ev == -1 && spdef_ev == -1); }
        bool isEmptyInput() const { return (hp_ev == -3 && def_ev == -3 && spdef_ev == -3); }
        bool isAborted() const { return (hp_ev == -4 && def_ev == -4 && spdef_ev == -4); }
};

#endif // DEFENSERESULT_HPP
