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
        void setButtonClickable(int row, int column);
        void eraseMove(bool checked);
        void openMoveWindowEdit(bool checked);
        void solveMove();
        void openMoveWindow(bool checked);
        void clear();
        void calculate();

    private:
        //ADD MOVE WINDOW
        MoveWindow* move_window;
        ResultWindow* result_window;
        //ADD RESULT WINDOW

        //DEFENDING POKEMON GROUPBOX
        QGridLayout* defending_pokemon_layout;
        QGroupBox* defending_pokemon_groupbox;
        QComboBox* defending_pokemon_species_combobox;
        QLabel* defending_pokemon_species_label;
        QComboBox* defending_pokemon_type1_combobox;
        QLabel* defending_pokemon_type1_label;
        QComboBox* defending_pokemon_type2_combobox;
        QLabel* defending_pokemon_type2_label;
        QComboBox* defending_pokemon_ability_combobox;
        QLabel* defending_pokemon_ability_label;
        QComboBox* defending_pokemon_nature_combobox;
        QLabel* defending_pokemon_nature_label;
        QComboBox* defending_pokemon_item_combobox;
        QLabel* defending_pokemon_item_label;
        QSpinBox* defending_pokemon_attack_spinbox;
        QLabel* defending_pokemon_attack_label;
        QSpinBox* defending_pokemon_spattack_spinbox;
        QLabel* defending_pokemon_spattack_label;
        QSpinBox* defending_pokemon_speed_spinbox;
        QLabel* defending_pokemon_speed_label;
        QLabel* defending_pokemon_hpiv_label;
        QSpinBox* defending_pokemon_hpiv_spinbox;
        QLabel* defending_pokemon_defiv_label;
        QSpinBox* defending_pokemon_defiv_spinbox;
        QLabel* defending_pokemon_spdefiv_label;
        QSpinBox* defending_pokemon_spdefiv_spinbox;

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

        std::vector<Turn> turns;
        std::vector<defense_modifier> modifiers;

        void createDefendingPokemonGroupBox();
        void createMovesGroupBox();

    public:
        MainWindow();

        const std::vector<QString>& getSpeciesNames() const { return species_names; }
        const std::vector<QString>& getNaturesNames() const { return natures_names; }
        const std::vector<QString>& getAbilitiesNames() const { return abilities_names; }
        const std::vector<QString>& getTypesNames() const { return types_names; }
        const std::vector<QString>& getItemsNames() const { return items_names; }
        const std::vector<QString>& getMovesNames() const { return moves_names; }
        void addTurn(const Turn& theTurn, const defense_modifier& theModifier);
};

#endif // MAINWINDOW_HPP
