#ifndef RESULTWINDOW_HPP
#define RESULTWINDOW_HPP

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>

#include "pokemon.hpp"
#include "turn.hpp"

class ResultWindow : public QDialog {
    private:
        QGroupBox* result_groupbox;
        QLabel* hp_evs;
        QLabel* def_evs;
        QLabel* spdef_evs;
        QTextEdit* text_edit;

    public:
        ResultWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        void setResult(const Pokemon& theDefendingPokemon, const std::vector<Turn>& theTurns, const std::tuple<int, int, int>& theResult, const std::vector<float>& theRoll);
};

#endif // RESULTWINDOW_HPP
