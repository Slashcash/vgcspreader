#ifndef ATTACKRESULT_HPP
#define ATTACKRESULT_HPP

//this class is really simple and is mainly a getter/setter collection so don't bother about it

#include <vector>

class AttackResult {
    public:
        int atk_ev;
        int spatk_ev;
        std::vector<float> atk_ko_prob;
        std::vector<std::vector<float>> atk_damage_perc;
        std::vector<std::vector<int>> atk_damage_int;

        bool isValid() const { return (atk_ev != -2 && spatk_ev != -2); }
        bool isEmpty() const { return (atk_ev == -1 && spatk_ev == -1); }
        bool isEmptyInput() const { return (atk_ev == -3 && spatk_ev == -3); }
        bool isAborted() const { return (atk_ev == -4 && spatk_ev == -4); }
};

#endif // ATTACKRESULT_HPP
