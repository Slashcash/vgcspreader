#ifndef RESULTWINDOW_HPP
#define RESULTWINDOW_HPP

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>
#include <vector>

#include "pokemon.hpp"
#include "turn.hpp"
#include "defenseresult.hpp"
#include "attackresult.hpp"

class ResultWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setResultType(int index);

    private:
        QGroupBox* result_groupbox;
        QGroupBox* calc_groupbox;

        //here because they need to be accessed by different functions
        Pokemon* result_pokemon; //since we can't blank construct a pokemon
        std::vector<defense_modifier> result_def_modifier;
        std::vector<attack_modifier> result_atk_modifier;
        std::vector<Turn> result_def_turns;
        std::vector<Turn> result_atk_turns;
        std::vector<Pokemon> result_pkmn_attack;
        DefenseResult result_def;
        AttackResult result_atk;

        void createResultGroupBox();
        void createCalcGroupBox();
        void setResultDefense(const Pokemon& theDefendingPokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<Turn>& theTurns, const DefenseResult theResult, const int index);
        void setResultAttack(const Pokemon& theAttackingPokemon, const std::vector<Pokemon>& theDefendingPokemon, const std::vector<attack_modifier>& theAtkModifier, const std::vector<Turn>& theTurns, const AttackResult theResult, const int index);
        QString getAttackPokemon(const Pokemon& thePokemon, const Move& theMove);
        QString getDefendPokemon(const Pokemon& thePokemon, const std::tuple<int, int, int>& theResult, const defense_modifier& theDefModifier, const Move& theMove, const bool isDualDefense);
        QString getModifiers(const Turn& theTurn);
        QString getResults(const Turn& theTurn, const Pokemon& thePokemon, const float theRoll, const std::vector<float>& theDamagePerc);
        QString getCompleteString(const Turn& theTurn, const Pokemon& theDefendingPokemon, const bool isDualDefense, const float theRoll, const std::vector<float>& theDamagePerc); //THIS IS A MONSTER A BEAUTIFUL MONSTEEEEEER

    public:
        ResultWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        void setResult(const Pokemon& thePokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<attack_modifier>& theAtkModifier, const std::vector<Turn>& theDefendTurns, const std::vector<Turn>& theAttackTurns, const std::vector<Pokemon>& theDefPokemonInAtk, const DefenseResult theDefenseResult, const AttackResult theAttackResult); //MONSTROUS

        virtual ~ResultWindow() { delete result_pokemon; }
};

#endif // RESULTWINDOW_HPP
