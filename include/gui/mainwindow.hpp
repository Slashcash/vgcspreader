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

#include "movewindow.hpp"
#include "resultwindow.hpp"
#include "turn.hpp"
#include "pokemon.hpp"

class MainWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setDefendingPokemonSpecies(int index);
        void setDefendingPokemonForm(int index);
        void setButtonClickable(int row, int column);
        void eraseMove(bool checked);
        void openMoveWindowEdit(bool checked);
        void solveMove();
        void openMoveWindow(bool checked);
        void clear();
        void calculate();

    private:
        MoveWindow* move_window;
        ResultWindow* result_window;

        QGroupBox* defending_groupbox;

        //MOVES GROUPBOX
        QGridLayout* moves_layout;
        QGroupBox* moves_groupbox;
        QPushButton* moves_add_button;
        QPushButton* moves_edit_button;
        QPushButton* moves_delete_button;
        QTableWidget* moves_view;

        QDialogButtonBox* bottom_buttons;

        std::vector<QString> species_names;
        std::vector<QString> types_names;
        std::vector<QString> natures_names;
        std::vector<QString> abilities_names;
        std::vector<QString> items_names;
        std::vector<QString> moves_names;
        std::vector<QString> forms_names;

        std::vector<Turn> turns;
        std::vector<defense_modifier> modifiers;

        void createDefendingPokemonGroupBox();
        void createMovesGroupBox();

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
