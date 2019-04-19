#ifndef DEFENSERESULT_HPP
#define DEFENSERESULT_HPP

//this class is really simple and is mainly a getter/setter collection so don't bother about it

#include <vector>

enum ResultType {
    MINIMUM_EVS,
    HPFOCUS_EVS,
    RESULT_TYPE_NUM
};

class DefenseResult {
    public:
        std::vector<int> hp_ev;
        std::vector<int> def_ev;
        std::vector<int> spdef_ev;
        std::vector<std::vector<float>> def_ko_prob;
        std::vector<std::vector<std::vector<float>>> def_damage_perc;
        std::vector<std::vector<std::vector<int>>> def_damage_int; //we really should find a more suitable container :D

        bool isValid() const { return (hp_ev[0] != -2 && def_ev[0] != -2 && spdef_ev[0] != -2); }
        bool isEmpty() const { return (hp_ev[0] == -1 && def_ev[0] == -1 && spdef_ev[0] == -1); }
        bool isEmptyInput() const { return (hp_ev[0] == -3 && def_ev[0] == -3 && spdef_ev[0] == -3); }
        bool isAborted() const { return (hp_ev[0] == -4 && def_ev[0] == -4 && spdef_ev[0] == -4); }
};

#endif // DEFENSERESULT_HPP
