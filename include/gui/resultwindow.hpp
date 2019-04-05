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

    public:
        ResultWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        void setResult(const Pokemon& theDefendingPokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<Turn>& theTurns, const std::tuple<int, int, int>& theResult, const std::vector<std::vector<float>>& theDamagePerc, const std::vector<std::vector<int>>& theDamageInt, const std::vector<float>& theRoll);
};

#endif // RESULTWINDOW_HPP
