#ifndef RESULTWINDOW_HPP
#define RESULTWINDOW_HPP

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>

#include "pokemon.hpp"
#include "turn.hpp"

class ResultWindow : public QDialog {
    private:
        QGroupBox* result_groupbox;
        QGroupBox* calc_groupbox;

        void createResultGroupBox();
        QString getAttackPokemon(const Pokemon& thePokemon, const Move& theMove);
        QString getDefendPokemon(const Pokemon& thePokemon, const std::tuple<int, int, int>& theResult, const defense_modifier& theDefModifier, const Move& theMove, const bool isDualDefense);
        QString getModifiers(const Turn& theTurn);
        QString getResults(const Turn& theTurn, const Pokemon& thePokemon, const float theRoll, const std::vector<float>& theDamagePerc);
        QString getCompleteString(const Turn& theTurn, const Pokemon& theDefendingPokemon, const std::tuple<int, int, int>& theResult, const defense_modifier& theDefModifier, const bool isDualDefense, const float theRoll, const std::vector<float>& theDamagePerc); //THIS IS A MONSTER A BEAUTIFUL MONSTEEEEEER

    public:
        ResultWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        void setResult(const Pokemon& theDefendingPokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<Turn>& theTurns, const std::tuple<int, int, int>& theResult, const std::vector<std::vector<float>>& theDamagePerc, const std::vector<float>& theRoll);
};

#endif // RESULTWINDOW_HPP
