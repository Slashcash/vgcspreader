#ifndef ATTACKRESULT_HPP
#define ATTACKRESULT_HPP

//this class is really simple and is mainly a getter/setter collection so don't bother about it

#include <vector>

class AttackResult {
    public:
        std::vector<int> atk_ev;
        std::vector<int> spatk_ev;
        std::vector<std::vector<float>> atk_ko_prob;
        std::vector<std::vector<std::vector<float>>> atk_damage_perc;
        std::vector<std::vector<std::vector<int>>> atk_damage_int; //we should probably find a better container :D

        bool isValid() const { return (atk_ev[0] != -2 && spatk_ev[0] != -2); }
        bool isEmpty() const { return (atk_ev[0] == -1 && spatk_ev[0] == -1); }
        bool isEmptyInput() const { return (atk_ev[0] == -3 && spatk_ev[0] == -3); }
        bool isAborted() const { return (atk_ev[0] == -4 && spatk_ev[0] == -4); }
};

#endif // ATTACKRESULT_HPP
