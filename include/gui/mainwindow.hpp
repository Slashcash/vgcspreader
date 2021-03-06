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

#include "defensemovewindow.hpp"
#include "attackmovewindow.hpp"
#include "resultwindow.hpp"
#include "alertwindow.hpp"
#include "presetwindow.hpp"
#include "turn.hpp"
#include "pokemon.hpp"
#include "tinyxml2.h"

typedef std::tuple<QString, Turn, defense_modifier> Preset;

class MainWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setDefendingPokemonSpecies(int index);
        void setDefendingPokemonForm(int index);
        void setButtonClickable(int row, int column);
        void eraseMove(bool checked);
        void addPreset(bool checked);
        void solveMoveDefense();
        void solveMoveAttack();
        void openMoveWindow(bool checked);
        void openMoveWindowEdit(bool checked);
        void moveTabChanged(int index);
        void clear(QAbstractButton* theButton);
        void calculate();
        void calculateStart();
        void calculateStop();
        void calculateFinished();

    private:
        DefenseMoveWindow* defense_move_window;
        AttackMoveWindow* attack_move_window;
        ResultWindow* result_window;
        AlertWindow* alert_window;
        PresetWindow* preset_window;

        QFutureWatcher<void> future_watcher;
        QFuture<FinalResult> future;
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
        std::vector<Turn> turns_atk;
        std::vector<defense_modifier> modifiers_def;
        std::vector<attack_modifier> modifiers_atk;
        std::vector<Pokemon> defending_pokemons_in_attack;

        void createDefendingPokemonGroupBox();
        void createMovesGroupBox();
        void openMoveWindowDefense();
        void openMoveWindowAttack();
        void openMoveWindowEditDefense();
        void openMoveWindowEditAttack();
        void LoadPresetsFromFile();

    protected:
        void reject();

    public:
        std::vector<Preset> presets; //HERE BECAUSE THIS PROGRAM IS A MESS
        tinyxml2::XMLDocument xml_preset; //THIS TOO

        MainWindow();

        const std::vector<QString>& getSpeciesNames() const { return species_names; }
        const std::vector<QString>& getFormsNames() const { return forms_names; }
        const std::vector<QString>& getNaturesNames() const { return natures_names; }
        const std::vector<QString>& getAbilitiesNames() const { return abilities_names; }
        const std::vector<QString>& getTypesNames() const { return types_names; }
        const std::vector<QString>& getItemsNames() const { return items_names; }
        const std::vector<QString>& getMovesNames() const { return moves_names; }
        void addDefenseTurn(const Turn& theTurn, const defense_modifier& theModifier);
        void addAttackTurn(const Turn& theTurn, const Pokemon& theDefendingPokemon, const attack_modifier& theModifier);
        void addAsPreset(const QString& theName, const Turn& theTurn, const defense_modifier& theDefModifier);
        void solveMovePreset(const int index);

        //MISC
        QString retrieveFormName(const int species, const int form);
};

#endif // MAINWINDOW_HPP
