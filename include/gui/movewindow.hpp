#ifndef MOVEWINDOW_HPP
#define MOVEWINDOW_HPP

#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QDialogButtonBox>

#include "turn.hpp"

class MoveWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setMove1(int index);
        void setSpecies1(int index);
        void setMove2(int index);
        void setSpecies2(int index);
        void activateAtk2(int state);
        void solveMove(void);

    private:
        QGroupBox* atk1_groupbox;

        //ATTACCKING POKEMON NO1 GROUPBOX
        QGridLayout* atk1_pokemon_layout;
        QGroupBox* atk1_pokemon_groupbox;
        QComboBox* atk1_pokemon_species_combobox;
        QLabel* atk1_pokemon_species_label;
        QComboBox* atk1_pokemon_type1_combobox;
        QLabel* atk1_pokemon_type1_label;
        QComboBox* atk1_pokemon_type2_combobox;
        QLabel* atk1_pokemon_type2_label;
        QComboBox* atk1_pokemon_ability_combobox;
        QLabel* atk1_pokemon_ability_label;
        QComboBox* atk1_pokemon_nature_combobox;
        QLabel* atk1_pokemon_nature_label;
        QComboBox* atk1_pokemon_item_combobox;
        QLabel* atk1_pokemon_item_label;

        QSpinBox* atk1_pokemon_attack_iv_spinbox;
        QLabel* atk1_pokemon_attack_iv_label;
        QSpinBox* atk1_pokemon_spattack_iv_spinbox;
        QLabel* atk1_pokemon_spattack_iv_label;
        QSpinBox* atk1_pokemon_attack_ev_spinbox;
        QLabel* atk1_pokemon_attack_ev_label;
        QSpinBox* atk1_pokemon_attack_modifier_spinbox;
        QLabel* atk1_pokemon_attack_modifier_label;
        QLabel* atk1_pokemon_spattack_ev_label;
        QSpinBox* atk1_pokemon_spattack_ev_spinbox;
        QLabel* atk1_pokemon_spattack_modifier_label;
        QSpinBox* atk1_pokemon_spattack_modifier_spinbox;

        QComboBox* atk1_pokemon_moves_combobox;
        QLabel* atk1_pokemon_moves_label;
        QComboBox* atk1_pokemon_movetype_combobox;
        QComboBox* atk1_pokemon_movecategory_combobox;
        QSpinBox* atk1_pokemon_movebp_spinbox;
        QCheckBox* atk1_pokemon_crit_checkbox;
        QLabel* atk1_pokemon_crit_label;
        QCheckBox* atk1_pokemon_z_checkbox;
        QLabel* atk1_pokemon_z_label;

        //ATTACCKING POKEMON NO2 GROUPBOX
        QGridLayout* atk2_pokemon_layout;
        QGroupBox* atk2_pokemon_groupbox;
        QCheckBox* atk2_pokemon_activated;
        QComboBox* atk2_pokemon_species_combobox;
        QLabel* atk2_pokemon_species_label;
        QComboBox* atk2_pokemon_type1_combobox;
        QLabel* atk2_pokemon_type1_label;
        QComboBox* atk2_pokemon_type2_combobox;
        QLabel* atk2_pokemon_type2_label;
        QComboBox* atk2_pokemon_ability_combobox;
        QLabel* atk2_pokemon_ability_label;
        QComboBox* atk2_pokemon_nature_combobox;
        QLabel* atk2_pokemon_nature_label;
        QComboBox* atk2_pokemon_item_combobox;
        QLabel* atk2_pokemon_item_label;

        QSpinBox* atk2_pokemon_attack_iv_spinbox;
        QLabel* atk2_pokemon_attack_iv_label;
        QSpinBox* atk2_pokemon_spattack_iv_spinbox;
        QLabel* atk2_pokemon_spattack_iv_label;
        QSpinBox* atk2_pokemon_attack_ev_spinbox;
        QLabel* atk2_pokemon_attack_ev_label;
        QSpinBox* atk2_pokemon_attack_modifier_spinbox;
        QLabel* atk2_pokemon_attack_modifier_label;
        QLabel* atk2_pokemon_spattack_ev_label;
        QSpinBox* atk2_pokemon_spattack_ev_spinbox;
        QLabel* atk2_pokemon_spattack_modifier_label;
        QSpinBox* atk2_pokemon_spattack_modifier_spinbox;

        QComboBox* atk2_pokemon_moves_combobox;
        QLabel* atk2_pokemon_moves_label;
        QComboBox* atk2_pokemon_movetype_combobox;
        QComboBox* atk2_pokemon_movecategory_combobox;
        QSpinBox* atk2_pokemon_movebp_spinbox;
        QCheckBox* atk2_pokemon_crit_checkbox;
        QLabel* atk2_pokemon_crit_label;
        QCheckBox* atk2_pokemon_z_checkbox;
        QLabel* atk2_pokemon_z_label;

        //DEFENDING GROUPBOX
        QGroupBox* defending_pokemon_groupbox;
        QSpinBox* defending_pokemon_def_modifier_spinbox;
        QSpinBox* defending_pokemon_spdef_modifier_spinbox;
        QSpinBox* defending_pokemon_hp_modifier_spinbox;
        QSpinBox* defending_pokemon_hits_modifier_spinbox;

        //WEATHER
        QGroupBox* atk_pokemon_weather_groupbox;
        QRadioButton* atk_pokemon_weather_none;
        QRadioButton* atk_pokemon_weather_sun;
        QRadioButton* atk_pokemon_weather_rain;

        //TERRAIN
        QGroupBox* atk_pokemon_terrain_groupbox;
        QRadioButton* atk_pokemon_terrain_none;
        QRadioButton* atk_pokemon_terrain_psychic;
        QRadioButton* atk_pokemon_terrain_electric;
        QRadioButton* atk_pokemon_terrain_misty;
        QRadioButton* atk_pokemon_terrain_grassy;

        bool edit_mode;

        void createAtk1GroupBox();
        void createAtk2GroupBox();
        void createDefendingGroupBox();

    public:
        MoveWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        QDialogButtonBox* bottom_button_box; //HERE BECAUSE IT HAS TO BE ACCESSED BY MAINWINDOW

        void setAsBlank();
        void setAsTurn(const Turn& theTurn, const defense_modifier& theDefenseModifier);
        bool isEditMode() { return edit_mode; }

        void setEditMode(const bool theValue) { edit_mode = theValue; }
};

#endif // MOVEWINDOW_HPP
