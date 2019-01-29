#include "movewindow.hpp"

#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QButtonGroup>
#include <QDebug>

#include <tuple>

#include "mainwindow.hpp"
#include "move.hpp"
#include "turn.hpp"
#include "items.hpp"

MoveWindow::MoveWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    QVBoxLayout* main_layout = new QVBoxLayout;

    edit_mode = false;

    createAtk1GroupBox();
    createAtk2GroupBox();
    createDefendingGroupBox();
    main_layout->addWidget(atk1_pokemon_groupbox);
    main_layout->addWidget(atk2_pokemon_groupbox);
    main_layout->addWidget(defending_pokemon_groupbox);

    bottom_button_box = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    main_layout->addWidget(bottom_button_box, Qt::AlignRight);

    setLayout(main_layout);

    //SIGNALS
    connect(bottom_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bottom_button_box, SIGNAL(accepted()), this, SLOT(solveMove(void)));
    connect(bottom_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void MoveWindow::createAtk1GroupBox() {
    atk1_pokemon_groupbox = new QGroupBox(tr("Attacking Pokemon 1:"));
    atk1_pokemon_layout = new QGridLayout;

    //ATK1 SPECIES
    atk1_pokemon_species_label = new QLabel;
    atk1_pokemon_species_label->setText(tr("Species"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_species_label, 0, 0,  Qt::AlignRight);

    atk1_pokemon_species_combobox = new QComboBox;
    auto atk1_species_buffer = ((MainWindow*)parentWidget())->getSpeciesNames();
    for( auto it = atk1_species_buffer.begin(); it < atk1_species_buffer.end(); it++ ) atk1_pokemon_species_combobox->addItem(*it);
    atk1_pokemon_layout->addWidget(atk1_pokemon_species_combobox, 0, 1);

    //ATK1 TYPE1
    Pokemon selected_pokemon(atk1_pokemon_species_combobox->currentIndex()+1);

    atk1_pokemon_type1_label = new QLabel;
    atk1_pokemon_type1_label->setText(tr("Type 1:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_type1_label, 1, 0, Qt::AlignRight);

    atk1_pokemon_type1_combobox = new QComboBox;
    auto atk1_type1_buffer = ((MainWindow*)parentWidget())->getTypesNames();
    for( auto it = atk1_type1_buffer.begin(); it < atk1_type1_buffer.end(); it++ ) atk1_pokemon_type1_combobox->addItem(*it);
    atk1_pokemon_layout->addWidget(atk1_pokemon_type1_combobox, 1, 1);
    atk1_pokemon_type1_combobox->setCurrentIndex(selected_pokemon.getTypes()[0]);

    //ATK1 TYPE2
    atk1_pokemon_type2_label = new QLabel;
    atk1_pokemon_type2_label->setText(tr("Type 2:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_type2_label, 2, 0, Qt::AlignRight);

    atk1_pokemon_type2_combobox = new QComboBox;
    for( auto it = atk1_type1_buffer.begin(); it < atk1_type1_buffer.end(); it++ ) atk1_pokemon_type2_combobox->addItem(*it);
    atk1_pokemon_layout->addWidget(atk1_pokemon_type2_combobox, 2, 1);
    atk1_pokemon_type2_combobox->setCurrentIndex(selected_pokemon.getTypes()[1]);

    if( selected_pokemon.getTypes()[0] == selected_pokemon.getTypes()[1] ) { atk1_pokemon_type2_label->setVisible(false); atk1_pokemon_type2_combobox->setVisible(false); }

    //ATK1 NATURE
    atk1_pokemon_nature_label = new QLabel;
    atk1_pokemon_nature_label->setText(tr("Nature:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_nature_label, 3, 0, Qt::AlignRight);

    atk1_pokemon_nature_combobox = new QComboBox;

    auto atk1_natures_buffer = ((MainWindow*)parentWidget())->getNaturesNames();
    for( auto it = atk1_natures_buffer.begin(); it < atk1_natures_buffer.end(); it++ ) atk1_pokemon_nature_combobox->addItem(*it);
    atk1_pokemon_layout->addWidget(atk1_pokemon_nature_combobox, 3, 1);

    //ATK1 ABILITY
    atk1_pokemon_ability_label = new QLabel;
    atk1_pokemon_ability_label->setText(tr("Ability:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_ability_label, 4, 0, Qt::AlignRight);

    atk1_pokemon_ability_combobox = new QComboBox;
    auto atk1_abilities_buffer = ((MainWindow*)parentWidget())->getAbilitiesNames();
    for( auto it = atk1_abilities_buffer.begin(); it < atk1_abilities_buffer.end(); it++ ) atk1_pokemon_ability_combobox->addItem(*it);
    atk1_pokemon_layout->addWidget(atk1_pokemon_ability_combobox, 4, 1);

    atk1_pokemon_ability_combobox->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0]);

    //ATK1 ITEM
    atk1_pokemon_item_label = new QLabel;
    atk1_pokemon_item_label->setText(tr("Item:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_item_label, 5, 0, Qt::AlignRight);

    atk1_pokemon_item_combobox = new QComboBox;
    auto atk1_items_buffer = ((MainWindow*)parentWidget())->getItemsNames();
    for( auto it = atk1_items_buffer.begin(); it < atk1_items_buffer.end(); it++ ) atk1_pokemon_item_combobox->addItem(*it);
    atk1_pokemon_layout->addWidget(atk1_pokemon_item_combobox, 5, 1);

    //ATK1 ATK IV
    //spacer
    QSpacerItem* spacer = new QSpacerItem(8, 0);

    atk1_pokemon_layout->addItem(spacer, 0 , 2);
    atk1_pokemon_attack_iv_label = new QLabel;
    atk1_pokemon_attack_iv_label->setText(tr("Attack IV:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_attack_iv_label, 0, 3, Qt::AlignRight);

    atk1_pokemon_attack_iv_spinbox = new QSpinBox;
    atk1_pokemon_attack_iv_spinbox->setRange(0, 31);
    atk1_pokemon_layout->addWidget(atk1_pokemon_attack_iv_spinbox, 0, 4);
    atk1_pokemon_attack_iv_spinbox->setValue(31);

    //ATK1 ATK EV
    atk1_pokemon_attack_ev_label = new QLabel;
    atk1_pokemon_attack_ev_label->setText(tr("Attack EV:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_attack_ev_label, 0, 5, Qt::AlignRight);

    atk1_pokemon_attack_ev_spinbox = new QSpinBox;
    atk1_pokemon_attack_ev_spinbox->setRange(0, 252);
    atk1_pokemon_layout->addWidget(atk1_pokemon_attack_ev_spinbox, 0, 6);

    //ATK1 ATK MODIFIER
    atk1_pokemon_attack_modifier_label = new QLabel;
    atk1_pokemon_attack_modifier_label->setText(tr("Attack Modifier:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_attack_modifier_label, 0, 7, Qt::AlignRight);

    atk1_pokemon_attack_modifier_spinbox = new QSpinBox;
    atk1_pokemon_attack_modifier_spinbox->setRange(-6, 6);
    atk1_pokemon_layout->addWidget(atk1_pokemon_attack_modifier_spinbox, 0, 8);    //ATK1 ATK IV

    //ATK1 SPATK IV
    atk1_pokemon_layout->addItem(spacer, 1 , 2);
    atk1_pokemon_spattack_iv_label = new QLabel;
    atk1_pokemon_spattack_iv_label->setText(tr("Sp. Attack IV:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_spattack_iv_label, 1, 3, Qt::AlignRight);

    atk1_pokemon_spattack_iv_spinbox = new QSpinBox;
    atk1_pokemon_spattack_iv_spinbox->setRange(0, 31);
    atk1_pokemon_layout->addWidget(atk1_pokemon_spattack_iv_spinbox, 1, 4);
    atk1_pokemon_spattack_iv_spinbox->setValue(31);

    //ATK1 SPATK EV
    atk1_pokemon_spattack_ev_label = new QLabel;
    atk1_pokemon_spattack_ev_label->setText(tr("Sp. Attack EV:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_spattack_ev_label, 1, 5, Qt::AlignRight);

    atk1_pokemon_spattack_ev_spinbox = new QSpinBox;
    atk1_pokemon_spattack_ev_spinbox->setRange(0, 252);
    atk1_pokemon_layout->addWidget(atk1_pokemon_spattack_ev_spinbox, 1, 6);

    //ATK1 SPATK MODIFIER
    atk1_pokemon_spattack_modifier_label = new QLabel;
    atk1_pokemon_spattack_modifier_label->setText(tr("Sp. Attack Modifier:"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_spattack_modifier_label, 1, 7, Qt::AlignRight);

    atk1_pokemon_spattack_modifier_spinbox = new QSpinBox;
    atk1_pokemon_spattack_modifier_spinbox->setRange(-6, 6);
    atk1_pokemon_layout->addWidget(atk1_pokemon_spattack_modifier_spinbox, 1, 8);

    atk1_pokemon_groupbox->setLayout(atk1_pokemon_layout);

    //MOVE NAMES
    atk1_pokemon_moves_label = new QLabel;
    atk1_pokemon_moves_label->setText(tr("Move"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_moves_label, 6, 0,  Qt::AlignRight);

    //loading moves names
    atk1_pokemon_moves_combobox = new QComboBox;

    auto atk1_moves_buffer = ((MainWindow*)parentWidget())->getMovesNames();
    for( auto it = atk1_moves_buffer.begin(); it < atk1_moves_buffer.end(); it++ ) atk1_pokemon_moves_combobox->addItem(*it);

    atk1_pokemon_layout->addWidget(atk1_pokemon_moves_combobox, 6, 1);

    //MOVE TYPE
    Move selected_move((Moves)atk1_pokemon_moves_combobox->currentIndex());

    atk1_pokemon_movetype_combobox = new QComboBox();
    for( auto it = atk1_type1_buffer.begin(); it < atk1_type1_buffer.end(); it++ ) atk1_pokemon_movetype_combobox->addItem(*it);
    atk1_pokemon_layout->addWidget(atk1_pokemon_movetype_combobox, 6, 2);

    atk1_pokemon_movetype_combobox->setCurrentIndex(selected_move.getMoveType());

    //MOVE CATEGORY
    atk1_pokemon_movecategory_combobox = new QComboBox();
    atk1_pokemon_movecategory_combobox->addItem(tr("Physical"));
    atk1_pokemon_movecategory_combobox->addItem(tr("Special"));
    atk1_pokemon_layout->addWidget(atk1_pokemon_movecategory_combobox, 6, 3);

    atk1_pokemon_movecategory_combobox->setCurrentIndex(selected_move.getMoveCategory());
    //MOVE BP
    atk1_pokemon_movebp_spinbox = new QSpinBox;
    atk1_pokemon_movebp_spinbox->setRange(1, 999);
    atk1_pokemon_movebp_spinbox->setValue(selected_move.getBasePower());
    atk1_pokemon_layout->addWidget(atk1_pokemon_movebp_spinbox, 6, 4);


    //MOVE MODIFIERS
    QHBoxLayout* move_modifiers_layout = new QHBoxLayout;

    //MOVE CRIT
    atk1_pokemon_crit_checkbox = new QCheckBox;
    move_modifiers_layout->addWidget(atk1_pokemon_crit_checkbox);

    atk1_pokemon_crit_label = new QLabel;
    atk1_pokemon_crit_label->setText(tr("Crit"));
    move_modifiers_layout->addWidget(atk1_pokemon_crit_label, Qt::AlignLeft);

    atk1_pokemon_layout->addLayout(move_modifiers_layout, 6, 5);

    //Z
    atk1_pokemon_z_checkbox = new QCheckBox;
    move_modifiers_layout->addWidget(atk1_pokemon_z_checkbox);

    atk1_pokemon_z_label = new QLabel;
    atk1_pokemon_z_label->setText(tr("Z"));
    move_modifiers_layout->addWidget(atk1_pokemon_z_label, Qt::AlignLeft);

    //WEATHER
    QHBoxLayout* atk1_weather_layout = new QHBoxLayout;
    QLabel* atk1_weather_label = new QLabel(tr("Weather:"));
    atk1_weather_layout->addWidget(atk1_weather_label);

    QButtonGroup* atk1_weather_button_group = new QButtonGroup;
    //none
    atk1_pokemon_weather_none = new QRadioButton(tr("None"));
    atk1_weather_button_group->addButton(atk1_pokemon_weather_none);
    atk1_weather_layout->addWidget(atk1_pokemon_weather_none);
    atk1_pokemon_weather_none->setChecked(true);
    //sun
    atk1_pokemon_weather_sun = new QRadioButton(tr("Sun"));
    atk1_weather_button_group->addButton(atk1_pokemon_weather_sun);
    atk1_weather_layout->addWidget(atk1_pokemon_weather_sun);
    //rain
    atk1_pokemon_weather_rain = new QRadioButton(tr("Rain"));
    atk1_weather_button_group->addButton(atk1_pokemon_weather_rain);
    atk1_weather_layout->addWidget(atk1_pokemon_weather_rain);

    atk1_pokemon_layout->addLayout(atk1_weather_layout, 7, 1, 1, 2);

    //SIGNALS
    connect(atk1_pokemon_moves_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMove1(int)));
    connect(atk1_pokemon_species_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSpecies1(int)));
}

void MoveWindow::createAtk2GroupBox() {
    atk2_pokemon_groupbox = new QGroupBox(tr("Attacking Pokemon 2:"));
    atk2_pokemon_layout = new QGridLayout;

    //ATK2 SPECIES
    atk2_pokemon_species_label = new QLabel;
    atk2_pokemon_species_label->setText(tr("Species"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_species_label, 0, 0,  Qt::AlignRight);

    atk2_pokemon_species_combobox = new QComboBox;
    auto atk2_species_buffer = ((MainWindow*)parentWidget())->getSpeciesNames();
    for( auto it = atk2_species_buffer.begin(); it < atk2_species_buffer.end(); it++ ) atk2_pokemon_species_combobox->addItem(*it);
    atk2_pokemon_layout->addWidget(atk2_pokemon_species_combobox, 0, 1);

    //ATK2 ACTIVATED
    atk2_pokemon_activated = new QCheckBox;
    atk2_pokemon_layout->addWidget(atk2_pokemon_activated, 0, 2);
    atk2_pokemon_activated->setChecked(true);

    //ATK2 TYPE1
    Pokemon selected_pokemon(atk2_pokemon_species_combobox->currentIndex()+1);

    atk2_pokemon_type1_label = new QLabel;
    atk2_pokemon_type1_label->setText(tr("Type 1:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_type1_label, 1, 0, Qt::AlignRight);

    atk2_pokemon_type1_combobox = new QComboBox;
    auto atk2_type1_buffer = ((MainWindow*)parentWidget())->getTypesNames();
    for( auto it = atk2_type1_buffer.begin(); it < atk2_type1_buffer.end(); it++ ) atk2_pokemon_type1_combobox->addItem(*it);
    atk2_pokemon_layout->addWidget(atk2_pokemon_type1_combobox, 1, 1);
    atk2_pokemon_type1_combobox->setCurrentIndex(selected_pokemon.getTypes()[0]);

    //ATK2 TYPE2
    atk2_pokemon_type2_label = new QLabel;
    atk2_pokemon_type2_label->setText(tr("Type 2:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_type2_label, 2, 0, Qt::AlignRight);

    atk2_pokemon_type2_combobox = new QComboBox;
    for( auto it = atk2_type1_buffer.begin(); it < atk2_type1_buffer.end(); it++ ) atk2_pokemon_type2_combobox->addItem(*it);
    atk2_pokemon_layout->addWidget(atk2_pokemon_type2_combobox, 2, 1);
    atk2_pokemon_type2_combobox->setCurrentIndex(selected_pokemon.getTypes()[1]);

    if( selected_pokemon.getTypes()[0] == selected_pokemon.getTypes()[1] ) { atk2_pokemon_type2_label->setVisible(false); atk2_pokemon_type2_combobox->setVisible(false); }

    //ATK2 NATURE
    atk2_pokemon_nature_label = new QLabel;
    atk2_pokemon_nature_label->setText(tr("Nature:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_nature_label, 3, 0, Qt::AlignRight);

    atk2_pokemon_nature_combobox = new QComboBox;

    auto atk2_natures_buffer = ((MainWindow*)parentWidget())->getNaturesNames();
    for( auto it = atk2_natures_buffer.begin(); it < atk2_natures_buffer.end(); it++ ) atk2_pokemon_nature_combobox->addItem(*it);
    atk2_pokemon_layout->addWidget(atk2_pokemon_nature_combobox, 3, 1);

    //ATK2 ABILITY
    atk2_pokemon_ability_label = new QLabel;
    atk2_pokemon_ability_label->setText(tr("Ability:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_ability_label, 4, 0, Qt::AlignRight);

    atk2_pokemon_ability_combobox = new QComboBox;
    auto atk2_abilities_buffer = ((MainWindow*)parentWidget())->getAbilitiesNames();
    for( auto it = atk2_abilities_buffer.begin(); it < atk2_abilities_buffer.end(); it++ ) atk2_pokemon_ability_combobox->addItem(*it);
    atk2_pokemon_layout->addWidget(atk2_pokemon_ability_combobox, 4, 1);

    atk2_pokemon_ability_combobox->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0]);

    //ATK1 ITEM
    atk2_pokemon_item_label = new QLabel;
    atk2_pokemon_item_label->setText(tr("Item:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_item_label, 5, 0, Qt::AlignRight);

    atk2_pokemon_item_combobox = new QComboBox;
    auto atk2_items_buffer = ((MainWindow*)parentWidget())->getItemsNames();
    for( auto it = atk2_items_buffer.begin(); it < atk2_items_buffer.end(); it++ ) atk2_pokemon_item_combobox->addItem(*it);
    atk2_pokemon_layout->addWidget(atk2_pokemon_item_combobox, 5, 1);

    //ATK2 ATK IV
    //spacer
    QSpacerItem* spacer = new QSpacerItem(8, 0);

    atk2_pokemon_layout->addItem(spacer, 0 , 2);
    atk2_pokemon_attack_iv_label = new QLabel;
    atk2_pokemon_attack_iv_label->setText(tr("Attack IV:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_attack_iv_label, 0, 3, Qt::AlignRight);

    atk2_pokemon_attack_iv_spinbox = new QSpinBox;
    atk2_pokemon_attack_iv_spinbox->setRange(0, 31);
    atk2_pokemon_layout->addWidget(atk2_pokemon_attack_iv_spinbox, 0, 4);
    atk2_pokemon_attack_iv_spinbox->setValue(31);

    //ATK2 ATK EV
    atk2_pokemon_attack_ev_label = new QLabel;
    atk2_pokemon_attack_ev_label->setText(tr("Attack EV:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_attack_ev_label, 0, 5, Qt::AlignRight);

    atk2_pokemon_attack_ev_spinbox = new QSpinBox;
    atk2_pokemon_attack_ev_spinbox->setRange(0, 252);
    atk2_pokemon_layout->addWidget(atk2_pokemon_attack_ev_spinbox, 0, 6);

    //ATK2 ATK MODIFIER
    atk2_pokemon_attack_modifier_label = new QLabel;
    atk2_pokemon_attack_modifier_label->setText(tr("Attack Modifier:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_attack_modifier_label, 0, 7, Qt::AlignRight);

    atk2_pokemon_attack_modifier_spinbox = new QSpinBox;
    atk2_pokemon_attack_modifier_spinbox->setRange(-6, 6);
    atk2_pokemon_layout->addWidget(atk2_pokemon_attack_modifier_spinbox, 0, 8);    //ATK1 ATK IV

    //ATK1 SPATK IV
    atk2_pokemon_layout->addItem(spacer, 1 , 2);
    atk2_pokemon_spattack_iv_label = new QLabel;
    atk2_pokemon_spattack_iv_label->setText(tr("Sp. Attack IV:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_spattack_iv_label, 1, 3, Qt::AlignRight);

    atk2_pokemon_spattack_iv_spinbox = new QSpinBox;
    atk2_pokemon_spattack_iv_spinbox->setRange(0, 31);
    atk2_pokemon_layout->addWidget(atk2_pokemon_spattack_iv_spinbox, 1, 4);
    atk2_pokemon_spattack_iv_spinbox->setValue(31);

    //ATK2 SPATK EV
    atk2_pokemon_spattack_ev_label = new QLabel;
    atk2_pokemon_spattack_ev_label->setText(tr("Sp. Attack EV:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_spattack_ev_label, 1, 5, Qt::AlignRight);

    atk2_pokemon_spattack_ev_spinbox = new QSpinBox;
    atk2_pokemon_spattack_ev_spinbox->setRange(0, 252);
    atk2_pokemon_layout->addWidget(atk2_pokemon_spattack_ev_spinbox, 1, 6);

    //ATK1 SPATK MODIFIER
    atk2_pokemon_spattack_modifier_label = new QLabel;
    atk2_pokemon_spattack_modifier_label->setText(tr("Sp. Attack Modifier:"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_spattack_modifier_label, 1, 7, Qt::AlignRight);

    atk2_pokemon_spattack_modifier_spinbox = new QSpinBox;
    atk2_pokemon_spattack_modifier_spinbox->setRange(-6, 6);
    atk2_pokemon_layout->addWidget(atk2_pokemon_spattack_modifier_spinbox, 1, 8);

    atk2_pokemon_groupbox->setLayout(atk2_pokemon_layout);

    //MOVE NAMES
    atk2_pokemon_moves_label = new QLabel;
    atk2_pokemon_moves_label->setText(tr("Move"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_moves_label, 6, 0,  Qt::AlignRight);

    //loading moves names
    atk2_pokemon_moves_combobox = new QComboBox;

    auto atk2_moves_buffer = ((MainWindow*)parentWidget())->getMovesNames();
    for( auto it = atk2_moves_buffer.begin(); it < atk2_moves_buffer.end(); it++ ) atk2_pokemon_moves_combobox->addItem(*it);

    atk2_pokemon_layout->addWidget(atk2_pokemon_moves_combobox, 6, 1);

    //MOVE TYPE
    Move selected_move((Moves)atk2_pokemon_moves_combobox->currentIndex());

    atk2_pokemon_movetype_combobox = new QComboBox();
    for( auto it = atk2_type1_buffer.begin(); it < atk2_type1_buffer.end(); it++ ) atk2_pokemon_movetype_combobox->addItem(*it);
    atk2_pokemon_layout->addWidget(atk2_pokemon_movetype_combobox, 6, 2);

    atk2_pokemon_movetype_combobox->setCurrentIndex(selected_move.getMoveType());

    //MOVE CATEGORY
    atk2_pokemon_movecategory_combobox = new QComboBox();
    atk2_pokemon_movecategory_combobox->addItem(tr("Physical"));
    atk2_pokemon_movecategory_combobox->addItem(tr("Special"));
    atk2_pokemon_layout->addWidget(atk2_pokemon_movecategory_combobox, 6, 3);

    atk2_pokemon_movecategory_combobox->setCurrentIndex(selected_move.getMoveCategory());
    //MOVE BP
    atk2_pokemon_movebp_spinbox = new QSpinBox;
    atk2_pokemon_movebp_spinbox->setRange(1, 999);
    atk2_pokemon_movebp_spinbox->setValue(selected_move.getBasePower());
    atk2_pokemon_layout->addWidget(atk2_pokemon_movebp_spinbox, 6, 4);


    //MOVE MODIFIERS
    QHBoxLayout* move_modifiers_layout = new QHBoxLayout;

    //MOVE CRIT
    atk2_pokemon_crit_checkbox = new QCheckBox;
    move_modifiers_layout->addWidget(atk2_pokemon_crit_checkbox);

    atk2_pokemon_crit_label = new QLabel;
    atk2_pokemon_crit_label->setText(tr("Crit"));
    move_modifiers_layout->addWidget(atk2_pokemon_crit_label, Qt::AlignLeft);

    //Z
    atk2_pokemon_z_checkbox = new QCheckBox;
    move_modifiers_layout->addWidget(atk2_pokemon_z_checkbox);

    atk2_pokemon_z_label = new QLabel;
    atk2_pokemon_z_label->setText(tr("Z"));
    move_modifiers_layout->addWidget(atk2_pokemon_z_label, Qt::AlignLeft);

    atk2_pokemon_layout->addLayout(move_modifiers_layout, 6, 5);

    //WEATHER
    QHBoxLayout* atk2_weather_layout = new QHBoxLayout;
    QLabel* atk2_weather_label = new QLabel(tr("Weather:"));
    atk2_weather_layout->addWidget(atk2_weather_label);

    QButtonGroup* atk2_weather_button_group = new QButtonGroup;
    //none
    atk2_pokemon_weather_none = new QRadioButton(tr("None"));
    atk2_weather_button_group->addButton(atk2_pokemon_weather_none);
    atk2_weather_layout->addWidget(atk2_pokemon_weather_none);
    atk2_pokemon_weather_none->setChecked(true);
    //sun
    atk2_pokemon_weather_sun = new QRadioButton(tr("Sun"));
    atk2_weather_button_group->addButton(atk2_pokemon_weather_sun);
    atk2_weather_layout->addWidget(atk2_pokemon_weather_sun);
    //rain
    atk2_pokemon_weather_rain = new QRadioButton(tr("Rain"));
    atk2_weather_button_group->addButton(atk2_pokemon_weather_rain);
    atk2_weather_layout->addWidget(atk2_pokemon_weather_rain);

    atk2_pokemon_layout->addLayout(atk2_weather_layout, 7, 1, 1, 2);

    //SIGNALS
    connect(atk2_pokemon_moves_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMove2(int)));
    connect(atk2_pokemon_species_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSpecies2(int)));
    connect(atk2_pokemon_activated, SIGNAL(stateChanged(int)), this, SLOT(activateAtk2(int)));
}

void MoveWindow::createDefendingGroupBox() {
    defending_pokemon_groupbox = new QGroupBox("Defending Pokemon Modifiers:");

    QHBoxLayout* defending_pokemon_layout = new QHBoxLayout;
    defending_pokemon_groupbox->setLayout(defending_pokemon_layout);

    QLabel* defending_pokemon_def_modifier_label = new QLabel;
    defending_pokemon_def_modifier_label->setText(tr("Defense modifier"));
    defending_pokemon_layout->addWidget(defending_pokemon_def_modifier_label);

    defending_pokemon_def_modifier_spinbox = new QSpinBox;
    defending_pokemon_def_modifier_spinbox->setRange(-6, 6);
    defending_pokemon_layout->addWidget(defending_pokemon_def_modifier_spinbox, Qt::AlignLeft);

    QLabel* defending_pokemon_spdef_modifier_label = new QLabel;
    defending_pokemon_spdef_modifier_label->setText(tr("Sp. Defense modifier"));
    defending_pokemon_layout->addWidget(defending_pokemon_spdef_modifier_label);

    defending_pokemon_spdef_modifier_spinbox = new QSpinBox;
    defending_pokemon_spdef_modifier_spinbox->setRange(-6, 6);
    defending_pokemon_layout->addWidget(defending_pokemon_spdef_modifier_spinbox, Qt::AlignLeft);

    QLabel* defending_pokemon_hp_modifier_label = new QLabel;
    defending_pokemon_hp_modifier_label->setText(tr("HP percentage:"));
    defending_pokemon_layout->addWidget(defending_pokemon_hp_modifier_label);

    defending_pokemon_hp_modifier_spinbox = new QSpinBox;
    defending_pokemon_hp_modifier_spinbox->setRange(0, 100);
    defending_pokemon_hp_modifier_spinbox->setSuffix("%");
    defending_pokemon_layout->addWidget(defending_pokemon_hp_modifier_spinbox, Qt::AlignLeft);

    QLabel* defending_pokemon_hits_modifier_label = new QLabel;
    defending_pokemon_hits_modifier_label->setText(tr("Number of hits:"));
    defending_pokemon_layout->addWidget(defending_pokemon_hits_modifier_label);

    defending_pokemon_hits_modifier_spinbox = new QSpinBox;
    defending_pokemon_hits_modifier_spinbox->setRange(2, 5);
    defending_pokemon_hits_modifier_spinbox->setSuffix("HKO");
    defending_pokemon_layout->addWidget(defending_pokemon_hits_modifier_spinbox, Qt::AlignLeft);
}

void MoveWindow::setMove1(int index) {
    Move selected_move((Moves)index);

    atk1_pokemon_movetype_combobox->setCurrentIndex(selected_move.getMoveType());
    atk1_pokemon_movecategory_combobox->setCurrentIndex(selected_move.getMoveCategory());
    atk1_pokemon_movebp_spinbox->setValue(selected_move.getBasePower());
    if( selected_move.isSignatureZ() ) atk1_pokemon_z_checkbox->setEnabled(false);
    else atk1_pokemon_z_checkbox->setEnabled(true);
}

void MoveWindow::setSpecies1(int index) {
    Pokemon selected_pokemon(index+1);

    atk1_pokemon_type1_combobox->setCurrentIndex(selected_pokemon.getTypes()[0]);
    atk1_pokemon_type2_combobox->setCurrentIndex(selected_pokemon.getTypes()[1]);
    atk1_pokemon_ability_combobox->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0]);

    if( selected_pokemon.getTypes()[0] == selected_pokemon.getTypes()[1] ) { atk1_pokemon_type2_label->setVisible(false); atk1_pokemon_type2_combobox->setVisible(false); }
    else { atk1_pokemon_type2_label->setVisible(true); atk1_pokemon_type2_combobox->setVisible(true); }
}

void MoveWindow::setMove2(int index) {
    Move selected_move((Moves)index);

    atk2_pokemon_movetype_combobox->setCurrentIndex(selected_move.getMoveType());
    atk2_pokemon_movecategory_combobox->setCurrentIndex(selected_move.getMoveCategory());
    atk2_pokemon_movebp_spinbox->setValue(selected_move.getBasePower());

    if( selected_move.isSignatureZ() ) atk2_pokemon_z_checkbox->setEnabled(false);
    else atk2_pokemon_z_checkbox->setEnabled(true);
}

void MoveWindow::setSpecies2(int index) {
    Pokemon selected_pokemon(index+1);

    atk2_pokemon_type1_combobox->setCurrentIndex(selected_pokemon.getTypes()[0]);
    atk2_pokemon_type2_combobox->setCurrentIndex(selected_pokemon.getTypes()[1]);
    atk2_pokemon_ability_combobox->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0]);

    if( selected_pokemon.getTypes()[0] == selected_pokemon.getTypes()[1] ) { atk2_pokemon_type2_label->setVisible(false); atk2_pokemon_type2_combobox->setVisible(false); }
    else { atk2_pokemon_type2_label->setVisible(true); atk2_pokemon_type2_combobox->setVisible(true); }
}

void MoveWindow::activateAtk2(int state) {
    if( state == Qt::Checked ) {
        atk2_pokemon_species_combobox->setEnabled(true);
        atk2_pokemon_type1_combobox->setEnabled(true);
        atk2_pokemon_type2_combobox->setEnabled(true);
        atk2_pokemon_nature_combobox->setEnabled(true);
        atk2_pokemon_ability_combobox->setEnabled(true);
        atk2_pokemon_item_combobox->setEnabled(true);
        atk2_pokemon_attack_iv_spinbox->setEnabled(true);
        atk2_pokemon_attack_ev_spinbox->setEnabled(true);
        atk2_pokemon_attack_modifier_spinbox->setEnabled(true);
        atk2_pokemon_spattack_iv_spinbox->setEnabled(true);
        atk2_pokemon_spattack_ev_spinbox->setEnabled(true);
        atk2_pokemon_spattack_modifier_spinbox->setEnabled(true);
        atk2_pokemon_moves_combobox->setEnabled(true);
        atk2_pokemon_movetype_combobox->setEnabled(true);
        atk2_pokemon_movecategory_combobox->setEnabled(true);
        atk2_pokemon_movebp_spinbox->setEnabled(true);
        atk2_pokemon_crit_checkbox->setEnabled(true);
        atk2_pokemon_weather_sun->setEnabled(true);
        atk2_pokemon_weather_rain->setEnabled(true);
        atk2_pokemon_weather_none->setEnabled(true);
    }

    else if( state == Qt::Unchecked ) {
        atk2_pokemon_species_combobox->setEnabled(false);
        atk2_pokemon_type1_combobox->setEnabled(false);
        atk2_pokemon_type2_combobox->setEnabled(false);
        atk2_pokemon_nature_combobox->setEnabled(false);
        atk2_pokemon_ability_combobox->setEnabled(false);
        atk2_pokemon_item_combobox->setEnabled(false);
        atk2_pokemon_attack_iv_spinbox->setEnabled(false);
        atk2_pokemon_attack_ev_spinbox->setEnabled(false);
        atk2_pokemon_attack_modifier_spinbox->setEnabled(false);
        atk2_pokemon_spattack_iv_spinbox->setEnabled(false);
        atk2_pokemon_spattack_ev_spinbox->setEnabled(false);
        atk2_pokemon_spattack_modifier_spinbox->setEnabled(false);
        atk2_pokemon_moves_combobox->setEnabled(false);
        atk2_pokemon_movetype_combobox->setEnabled(false);
        atk2_pokemon_movecategory_combobox->setEnabled(false);
        atk2_pokemon_movebp_spinbox->setEnabled(false);
        atk2_pokemon_crit_checkbox->setEnabled(false);
        atk2_pokemon_weather_sun->setEnabled(false);
        atk2_pokemon_weather_rain->setEnabled(false);
        atk2_pokemon_weather_none->setEnabled(false);
    }
}

void MoveWindow::solveMove(void) {
    Pokemon attacking1(atk1_pokemon_species_combobox->currentIndex()+1);

    attacking1.setIV(Stats::ATK, atk1_pokemon_attack_iv_spinbox->value());
    attacking1.setIV(Stats::SPATK, atk1_pokemon_spattack_iv_spinbox->value());
    attacking1.setEV(Stats::ATK, atk1_pokemon_attack_ev_spinbox->value());
    attacking1.setEV(Stats::SPATK, atk1_pokemon_spattack_ev_spinbox->value());
    attacking1.setModifier(Stats::ATK, atk1_pokemon_attack_modifier_spinbox->value());
    attacking1.setModifier(Stats::SPATK, atk1_pokemon_spattack_modifier_spinbox->value());

    attacking1.setType(0, (Type)atk1_pokemon_type1_combobox->currentIndex());
    attacking1.setType(1, (Type)atk1_pokemon_type2_combobox->currentIndex());
    attacking1.setNature((Stats::Nature)atk1_pokemon_nature_combobox->currentIndex());
    attacking1.setAbility((Ability)atk1_pokemon_ability_combobox->currentIndex());
    attacking1.setItem(Item(atk1_pokemon_item_combobox->currentIndex()));

    Move attacking1_move((Moves)atk1_pokemon_moves_combobox->currentIndex());
    attacking1_move.setMoveType((Type)atk1_pokemon_movetype_combobox->currentIndex());
    attacking1_move.setMoveCategory((Move::Category)atk1_pokemon_movecategory_combobox->currentIndex());
    attacking1_move.setBasePower(atk1_pokemon_movebp_spinbox->value());
    if( atk1_pokemon_crit_checkbox->checkState() == Qt::Checked ) attacking1_move.setCrit(true);
    else if( atk1_pokemon_crit_checkbox->checkState() == Qt::Unchecked ) attacking1_move.setCrit(false);

    if( atk1_pokemon_z_checkbox->checkState() == Qt::Checked ) attacking1_move.setZ(true);
    else if( atk1_pokemon_z_checkbox->checkState() == Qt::Unchecked ) attacking1_move.setZ(false);

    if( atk1_pokemon_weather_none->isChecked() ) attacking1_move.setWeather(Move::Weather::WEATHER_NONE);
    else if(  atk1_pokemon_weather_sun->isChecked() ) attacking1_move.setWeather(Move::Weather::SUN);
    else if(  atk1_pokemon_weather_rain->isChecked() ) attacking1_move.setWeather(Move::Weather::RAIN);

    Pokemon attacking2(atk2_pokemon_species_combobox->currentIndex()+1);

    attacking2.setIV(Stats::ATK, atk2_pokemon_attack_iv_spinbox->value());
    attacking2.setIV(Stats::SPATK, atk2_pokemon_spattack_iv_spinbox->value());
    attacking2.setEV(Stats::ATK, atk2_pokemon_attack_ev_spinbox->value());
    attacking2.setEV(Stats::SPATK, atk2_pokemon_spattack_ev_spinbox->value());
    attacking2.setModifier(Stats::ATK, atk2_pokemon_attack_modifier_spinbox->value());
    attacking2.setModifier(Stats::SPATK, atk2_pokemon_spattack_modifier_spinbox->value());

    attacking2.setType(0, (Type)atk2_pokemon_type1_combobox->currentIndex());
    attacking2.setType(1, (Type)atk2_pokemon_type2_combobox->currentIndex());
    attacking2.setNature((Stats::Nature)atk2_pokemon_nature_combobox->currentIndex());
    attacking2.setAbility((Ability)atk2_pokemon_ability_combobox->currentIndex());
    attacking2.setItem(Item(atk2_pokemon_item_combobox->currentIndex()));

    Move attacking2_move((Moves)atk2_pokemon_moves_combobox->currentIndex());
    attacking2_move.setMoveType((Type)atk2_pokemon_movetype_combobox->currentIndex());
    attacking2_move.setMoveCategory((Move::Category)atk2_pokemon_movecategory_combobox->currentIndex());
    attacking2_move.setBasePower(atk2_pokemon_movebp_spinbox->value());
    if( atk2_pokemon_crit_checkbox->checkState() == Qt::Checked ) attacking2_move.setCrit(true);
    else if( atk2_pokemon_crit_checkbox->checkState() == Qt::Unchecked ) attacking2_move.setCrit(false);

    if( atk2_pokemon_z_checkbox->checkState() == Qt::Checked ) attacking2_move.setZ(true);
    else if( atk2_pokemon_z_checkbox->checkState() == Qt::Unchecked ) attacking2_move.setZ(false);

    if( atk2_pokemon_weather_none->isChecked() ) attacking2_move.setWeather(Move::Weather::WEATHER_NONE);
    else if(  atk2_pokemon_weather_sun->isChecked() ) attacking2_move.setWeather(Move::Weather::SUN);
    else if(  atk2_pokemon_weather_rain->isChecked() ) attacking2_move.setWeather(Move::Weather::RAIN);

    Turn turn;
    turn.addMove(attacking1, attacking1_move);
    if( atk2_pokemon_activated->checkState() == Qt::Checked ) turn.addMove(attacking2, attacking2_move);
    turn.setHits(defending_pokemon_hits_modifier_spinbox->value()-1);

    defense_modifier def_mod = std::make_tuple(defending_pokemon_hp_modifier_spinbox->value(), defending_pokemon_def_modifier_spinbox->value(), defending_pokemon_spdef_modifier_spinbox->value());

    ((MainWindow*)parentWidget())->addTurn(turn, def_mod);
}

void MoveWindow::setAsBlank() {
    atk1_pokemon_species_combobox->setCurrentIndex(0);
    atk1_pokemon_nature_combobox->setCurrentIndex(0);
    atk1_pokemon_item_combobox->setCurrentIndex(0);

    atk1_pokemon_attack_iv_spinbox->setValue(31);
    atk1_pokemon_attack_ev_spinbox->setValue(0);
    atk1_pokemon_attack_modifier_spinbox->setValue(0);
    atk1_pokemon_spattack_iv_spinbox->setValue(31);
    atk1_pokemon_spattack_ev_spinbox->setValue(0);
    atk1_pokemon_spattack_modifier_spinbox->setValue(0);

    atk1_pokemon_moves_combobox->setCurrentIndex(0);
    atk1_pokemon_crit_checkbox->setChecked(false);
    atk1_pokemon_z_checkbox->setChecked(false);
    atk1_pokemon_weather_none->setChecked(true);

    atk2_pokemon_activated->setChecked(true);
    atk2_pokemon_species_combobox->setCurrentIndex(0);
    atk2_pokemon_nature_combobox->setCurrentIndex(0);
    atk2_pokemon_item_combobox->setCurrentIndex(0);

    atk2_pokemon_attack_iv_spinbox->setValue(31);
    atk2_pokemon_attack_ev_spinbox->setValue(0);
    atk2_pokemon_attack_modifier_spinbox->setValue(0);
    atk2_pokemon_spattack_iv_spinbox->setValue(31);
    atk2_pokemon_spattack_ev_spinbox->setValue(0);
    atk2_pokemon_spattack_modifier_spinbox->setValue(0);

    atk2_pokemon_moves_combobox->setCurrentIndex(0);
    atk2_pokemon_crit_checkbox->setChecked(false);
    atk2_pokemon_z_checkbox->setChecked(false);
    atk2_pokemon_weather_none->setChecked(true);

    defending_pokemon_def_modifier_spinbox->setValue(0);
    defending_pokemon_spdef_modifier_spinbox->setValue(0);
    defending_pokemon_hp_modifier_spinbox->setValue(100);
    defending_pokemon_hits_modifier_spinbox->setValue(2);
}

void MoveWindow::setAsTurn(const Turn &theTurn, const defense_modifier &theDefenseModifier) {
    atk1_pokemon_species_combobox->setCurrentIndex(theTurn.getMoves()[0].first.getPokedexNumber()-1);
    atk1_pokemon_nature_combobox->setCurrentIndex(theTurn.getMoves()[0].first.getNature());
    atk1_pokemon_item_combobox->setCurrentIndex(theTurn.getMoves()[0].first.getItem().getIndex());

    atk1_pokemon_attack_iv_spinbox->setValue(theTurn.getMoves()[0].first.getIV(Stats::ATK));
    atk1_pokemon_attack_ev_spinbox->setValue(theTurn.getMoves()[0].first.getEV(Stats::ATK));
    atk1_pokemon_attack_modifier_spinbox->setValue(theTurn.getMoves()[0].first.getModifier(Stats::ATK));
    atk1_pokemon_spattack_iv_spinbox->setValue(theTurn.getMoves()[0].first.getIV(Stats::SPATK));
    atk1_pokemon_spattack_ev_spinbox->setValue(theTurn.getMoves()[0].first.getEV(Stats::SPATK));
    atk1_pokemon_spattack_modifier_spinbox->setValue(theTurn.getMoves()[0].first.getModifier(Stats::SPATK));

    atk1_pokemon_moves_combobox->setCurrentIndex(theTurn.getMoves()[0].second.getMoveIndex());
    atk1_pokemon_movetype_combobox->setCurrentIndex(theTurn.getMoves()[0].second.getMoveType());
    atk1_pokemon_movecategory_combobox->setCurrentIndex(theTurn.getMoves()[0].second.getMoveCategory());
    atk1_pokemon_movebp_spinbox->setValue(theTurn.getMoves()[0].second.getBasePower());
    atk1_pokemon_crit_checkbox->setChecked(theTurn.getMoves()[0].second.isCrit());
    atk1_pokemon_z_checkbox->setChecked(theTurn.getMoves()[0].second.isZ());
    if( theTurn.getMoves()[0].second.getWeather() == Move::Weather::WEATHER_NONE ) atk1_pokemon_weather_none->setChecked(true);
    else if( theTurn.getMoves()[0].second.getWeather() == Move::Weather::SUN ) atk1_pokemon_weather_sun->setChecked(true);
    else if( theTurn.getMoves()[0].second.getWeather() == Move::Weather::RAIN ) atk1_pokemon_weather_rain->setChecked(true);

    if( theTurn.getMoveNum() > 1 ) {
        atk2_pokemon_species_combobox->setCurrentIndex(theTurn.getMoves()[1].first.getPokedexNumber()-1);
        atk2_pokemon_nature_combobox->setCurrentIndex(theTurn.getMoves()[1].first.getNature());
        atk2_pokemon_item_combobox->setCurrentIndex(theTurn.getMoves()[1].first.getItem().getIndex());

        atk2_pokemon_attack_iv_spinbox->setValue(theTurn.getMoves()[1].first.getIV(Stats::ATK));
        atk2_pokemon_attack_ev_spinbox->setValue(theTurn.getMoves()[1].first.getEV(Stats::ATK));
        atk2_pokemon_attack_modifier_spinbox->setValue(theTurn.getMoves()[1].first.getModifier(Stats::ATK));
        atk2_pokemon_spattack_iv_spinbox->setValue(theTurn.getMoves()[1].first.getIV(Stats::SPATK));
        atk2_pokemon_spattack_ev_spinbox->setValue(theTurn.getMoves()[1].first.getEV(Stats::SPATK));
        atk2_pokemon_spattack_modifier_spinbox->setValue(theTurn.getMoves()[1].first.getModifier(Stats::SPATK));

        atk2_pokemon_moves_combobox->setCurrentIndex(theTurn.getMoves()[1].second.getMoveIndex());
        atk2_pokemon_movetype_combobox->setCurrentIndex(theTurn.getMoves()[1].second.getMoveType());
        atk2_pokemon_movecategory_combobox->setCurrentIndex(theTurn.getMoves()[1].second.getMoveCategory());
        atk2_pokemon_movebp_spinbox->setValue(theTurn.getMoves()[1].second.getBasePower());
        atk2_pokemon_crit_checkbox->setChecked(theTurn.getMoves()[1].second.isCrit());
        atk2_pokemon_z_checkbox->setChecked(theTurn.getMoves()[1].second.isZ());
        if( theTurn.getMoves()[1].second.getWeather() == Move::Weather::WEATHER_NONE ) atk2_pokemon_weather_none->setChecked(true);
        else if( theTurn.getMoves()[1].second.getWeather() == Move::Weather::SUN ) atk2_pokemon_weather_sun->setChecked(true);
        else if( theTurn.getMoves()[1].second.getWeather() == Move::Weather::RAIN ) atk2_pokemon_weather_rain->setChecked(true);
    }

    else atk2_pokemon_activated->setChecked(false);

    defending_pokemon_def_modifier_spinbox->setValue(std::get<1>(theDefenseModifier));
    defending_pokemon_spdef_modifier_spinbox->setValue(std::get<2>(theDefenseModifier));
    defending_pokemon_hp_modifier_spinbox->setValue(std::get<0>(theDefenseModifier));
    defending_pokemon_hits_modifier_spinbox->setValue(theTurn.getHits());
}
