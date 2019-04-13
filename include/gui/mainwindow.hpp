#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <vector>

#include <QDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QFutureWatcher>

#include "movewindow.hpp"
#include "resultwindow.hpp"
#include "alertwindow.hpp"
#include "turn.hpp"
#include "pokemon.hpp"

class MainWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setDefendingPokemonSpecies(int index);
        void setDefendingPokemonForm(int index);
        void setButtonClickable(int row, int column);
        void eraseMove(bool checked);
        void openMoveWindowEditDefense(bool checked);
        void solveMove();
        void openMoveWindowDefense(bool checked);
        void clear(QAbstractButton* theButton);
        void calculate();
        void calculateStart();
        void calculateStop();
        void calculateFinished();

    private:
        MoveWindow* move_window;
        ResultWindow* result_window;
        AlertWindow* alert_window;

        QFutureWatcher<void> future_watcher;
        QFuture<DefenseResult> future;
        Pokemon* selected_pokemon; //here because when i calculate damages i need this, but it can be done better (see calculate() for explanation)

        QGroupBox* defending_groupbox;
        QGroupBox* moves_groupbox;
        QDialogButtonBox* bottom_buttons;

        std::vector<QString> species_names;
        std::vector<QString> types_names;
        std::vector<QString> natures_names;
        std::vector<QString> abilities_names;
        std::vector<QString> items_names;
        std::vector<QString> moves_names;
        std::vector<QString> forms_names;

        std::vector<Turn> turns_def;
        std::vector<defense_modifier> modifiers_def;

        void createDefendingPokemonGroupBox();
        void createMovesGroupBox();

    protected:
        void reject();

    public:
        MainWindow();

        const std::vector<QString>& getSpeciesNames() const { return species_names; }
        const std::vector<QString>& getFormsNames() const { return forms_names; }
        const std::vector<QString>& getNaturesNames() const { return natures_names; }
        const std::vector<QString>& getAbilitiesNames() const { return abilities_names; }
        const std::vector<QString>& getTypesNames() const { return types_names; }
        const std::vector<QString>& getItemsNames() const { return items_names; }
        const std::vector<QString>& getMovesNames() const { return moves_names; }
        void addTurn(const Turn& theTurn, const defense_modifier& theModifier);

        //MISC
        QString retrieveFormName(const int species, const int form);

};

#endif // MAINWINDOW_HPP
