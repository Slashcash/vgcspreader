#ifndef RESULTWINDOW_HPP
#define RESULTWINDOW_HPP

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>

#include "pokemon.hpp"
#include "turn.hpp"
#include "defenseresult.hpp"
#include "attackresult.hpp"

class ResultWindow : public QDialog {
    private:
        QGroupBox* result_groupbox;
        QGroupBox* calc_groupbox;

        void createResultGroupBox();
        void createCalcGroupBox();
        void setResultDefense(const Pokemon& theDefendingPokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<Turn>& theTurns, const DefenseResult theResult);
        void setResultAttack(const Pokemon& theAttackingPokemon, const std::vector<attack_modifier>& theAtkModifier, const std::vector<std::pair<Pokemon, Move>>& theTurns, const AttackResult theResult);
        QString getAttackPokemon(const Pokemon& thePokemon, const Move& theMove);
        QString getDefendPokemon(const Pokemon& thePokemon, const std::tuple<int, int, int>& theResult, const defense_modifier& theDefModifier, const Move& theMove, const bool isDualDefense);
        QString getModifiers(const Turn& theTurn);
        QString getResults(const Turn& theTurn, const Pokemon& thePokemon, const float theRoll, const std::vector<float>& theDamagePerc);
        QString getCompleteString(const Turn& theTurn, const Pokemon& theDefendingPokemon, const bool isDualDefense, const float theRoll, const std::vector<float>& theDamagePerc); //THIS IS A MONSTER A BEAUTIFUL MONSTEEEEEER

    public:
        ResultWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        void setResult(const Pokemon& thePokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<attack_modifier>& theAtkModifier, const std::vector<Turn>& theDefendTurns, const std::vector<std::pair<Pokemon, Move>>& theAttackTurns, const DefenseResult theDefenseResult, const AttackResult theAttackResult); //MONSTROUS
};

#endif // RESULTWINDOW_HPP
