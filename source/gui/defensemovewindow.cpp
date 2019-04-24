#include "defensemovewindow.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QButtonGroup>
#include <QFormLayout>
#include <QLineEdit>
#include <QPixmap>

#include <tuple>

#include "mainwindow.hpp"
#include "move.hpp"
#include "turn.hpp"
#include "items.hpp"

DefenseMoveWindow::DefenseMoveWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    QVBoxLayout* main_layout = new QVBoxLayout;

    //creating the tab
    tabs = new QTabWidget;
    main_layout->addWidget(tabs);

    edit_mode = false;

    createAtk1GroupBox();
    createAtk2GroupBox();
    createDefendingGroupBox();

    main_layout->addWidget(modifier_groupbox);
    main_layout->insertSpacing(2, 35);
    main_layout->addWidget(defending_groupbox);

    bottom_button_box = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    main_layout->addWidget(bottom_button_box, Qt::AlignRight);

    setLayout(main_layout);

    //SIGNALS
    connect(bottom_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bottom_button_box, SIGNAL(accepted()), this, SLOT(solveMove(void)));
    connect(bottom_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void DefenseMoveWindow::createAtk1GroupBox() {
    //the main horizontal layout for this window
    QHBoxLayout* atk1_layout = new QHBoxLayout;

    //the groupbox in which we encapsulate this window
    atk1_groupbox = new QGroupBox(tr("Attacking Pokemon 1:"));
    atk1_groupbox->setLayout(atk1_layout);

    //---SPECIES & TYPES---//

    //---species---//
    //the main layout for this entire section
    QVBoxLayout* species_types_layout = new QVBoxLayout;
    species_types_layout->setSpacing(1);
    species_types_layout->setAlignment(Qt::AlignVCenter);

    //---sprite---
    QHBoxLayout* sprite_layout = new QHBoxLayout;

    QLabel* sprite = new QLabel;
    sprite->setObjectName("atk1_sprite");
    sprite->setAlignment(Qt::AlignCenter);
    sprite_layout->addWidget(sprite);

    species_types_layout->addLayout(sprite_layout);

    //some spacing
    species_types_layout->insertSpacing(1, 10);

    //---species---//
    //the layout just for the species
    QHBoxLayout* species_layout = new QHBoxLayout;
    species_types_layout->addLayout(species_layout);

    //the combobox for all the species
    QComboBox* species = new QComboBox;
    species->setObjectName("atk1_species_combobox");

    //loading it with all the species name
    auto species_buffer = ((MainWindow*)parentWidget())->getSpeciesNames();
    for( auto it = species_buffer.begin(); it < species_buffer.end(); it++ ) species->addItem(*it);

    //some resizing
    int species_width = species->minimumSizeHint().width();
    species->setMaximumWidth(species_width);
    //adding it to the layout
    species_layout->addWidget(species);

    //---forms---//
    //the layout just for the forms
    QHBoxLayout* forms_layout = new QHBoxLayout;
    species_types_layout->addLayout(forms_layout);

    //the combobox for all the species
    QComboBox* forms = new QComboBox;
    forms->setObjectName("atk1_forms_combobox");

    //some resizing
    int forms_width = forms->minimumSizeHint().width();
    forms->setMaximumWidth(forms_width);
    //adding it to the layout
    forms_layout->addWidget(forms);

    //---types---//
    //the layout for the types
    QHBoxLayout* types_layout = new QHBoxLayout;

    //the comboboxes for the types
    QComboBox* types1 = new QComboBox;
    QComboBox* types2 = new QComboBox;
    types1->setObjectName("atk1_type1_combobox");
    types2->setObjectName("atk1_type2_combobox");

    //loading it with all the types names
    auto types_buffer = ((MainWindow*)parentWidget())->getTypesNames();
    for( auto it = types_buffer.begin(); it < types_buffer.end(); it++ ) { types1->addItem(*it); types2->addItem(*it); }

    //resizing them
    int types_width = species->minimumSizeHint().width();
    types1->setMaximumWidth(types_width);
    types2->setMaximumWidth(types_width);

    //adding them to the layout
    types_layout->addWidget(types1);
    types_layout->addWidget(types2);
    species_types_layout->addLayout(types_layout);

    //adding everything to the window
    atk1_layout->addLayout(species_types_layout);

    //and then some spacing
    atk1_layout->insertSpacing(1, 35);

    //---MAIN FORM---//
    QVBoxLayout* main_form_layout = new QVBoxLayout;

    //---information section---///
    QFormLayout* form_layout = new QFormLayout;
    main_form_layout->addLayout(form_layout);

    //NATURE
    //the combobox for the nature
    QComboBox* natures = new QComboBox;
    natures->setObjectName("atk1_nature_combobox");

    //populating it
    auto natures_buffer = ((MainWindow*)parentWidget())->getNaturesNames();
    for( auto it = natures_buffer.begin(); it < natures_buffer.end(); it++ ) natures->addItem(*it);

    form_layout->addRow(tr("Nature:"), natures);

    //ABILITY
    //the combobox for the ability
    QComboBox* abilities = new QComboBox;
    abilities->setObjectName("atk1_abilities_combobox");

    //populating it
    auto abilities_buffer = ((MainWindow*)parentWidget())->getAbilitiesNames();
    for( auto it = abilities_buffer.begin(); it < abilities_buffer.end(); it++ ) abilities->addItem(*it);

    //getting the abilities width (to be used later)
    int abilities_width = abilities->minimumSizeHint().width();

    form_layout->addRow(tr("Ability:"), abilities);

    //ITEM
    //the combobox for the item
    QComboBox* items = new QComboBox;
    items->setObjectName("atk1_items_combobox");

    //populating it
    auto items_buffer = ((MainWindow*)parentWidget())->getItemsNames();
    for( auto it = items_buffer.begin(); it < items_buffer.end(); it++ ) items->addItem(*it);

    form_layout->addRow(tr("Item:"), items);

    natures->setMaximumWidth(abilities_width);
    items->setMaximumWidth(abilities_width);
    abilities->setMaximumWidth(abilities_width);

    //some spacing
    main_form_layout->insertSpacing(1, 10);

    //---modifiers---//
    QHBoxLayout* modifiers_layout = new QHBoxLayout;
    modifiers_layout->setAlignment(Qt::AlignLeft);
    modifiers_layout->setSpacing(10);

    //iv
    QLabel* iv_label = new QLabel(tr("Atk IV:"));
    iv_label->setObjectName("atk1_iv_label");
    modifiers_layout->addWidget(iv_label);

    QSpinBox* iv = new QSpinBox;
    iv->setObjectName("atk1_iv_spinbox");
    iv->setRange(0, 31);
    iv->setValue(31);
    modifiers_layout->addWidget(iv);

    //ev
    QLabel* ev_label = new QLabel(tr("Atk EV:"));
    ev_label->setObjectName("atk1_ev_label");
    modifiers_layout->addWidget(ev_label);

    QSpinBox* ev = new QSpinBox;
    ev->setRange(0, 252);
    ev->setObjectName("atk1_ev_spinbox");
    modifiers_layout->addWidget(ev);

    //modifier
    QLabel* modifier_label = new QLabel(tr("Atk Modifier:"));
    modifier_label->setObjectName("atk1_modifier_label");
    modifiers_layout->addWidget(modifier_label);

    QSpinBox* modifier = new QSpinBox;
    modifier->setObjectName("atk1_modifier_spinbox");
    modifier->setRange(-6, 6);
    modifiers_layout->addWidget(modifier);

    main_form_layout->addLayout(modifiers_layout);

    //---moves---//
    QVBoxLayout* move_layout = new QVBoxLayout;
    move_layout->setAlignment(Qt::AlignLeft);

    //label
    QHBoxLayout* move_name_layout = new QHBoxLayout;
    move_name_layout->setAlignment(Qt::AlignLeft);
    QLabel* move_name_label = new QLabel(tr("Move:"));
    move_name_layout->addWidget(move_name_label);

    //name
    //the combobox for the move
    QComboBox* moves = new QComboBox;
    moves->setObjectName("atk1_moves_combobox");

    //populating it
    auto moves_buffer = ((MainWindow*)parentWidget())->getMovesNames();
    for( auto it = moves_buffer.begin(); it < moves_buffer.end(); it++ ) moves->addItem(*it);

    move_name_layout->addWidget(moves);

    move_layout->addLayout(move_name_layout);

    //target
    QHBoxLayout* move_info_layout = new QHBoxLayout;

    QComboBox* target = new QComboBox;
    target->setObjectName("atk1_target_combobox");

    //populating it
    target->addItem(tr("Single Target"));
    target->addItem(tr("Double Target"));

    move_info_layout->addWidget(target);

    //type
    QComboBox* move_types = new QComboBox;
    move_types->setObjectName("atk1_movetypes_combobox");

    //populating it
    auto movetypes_buffer = ((MainWindow*)parentWidget())->getTypesNames();
    for( auto it = movetypes_buffer.begin(); it < movetypes_buffer.end(); it++ ) move_types->addItem(*it);

    move_info_layout->addWidget(move_types);

    move_layout->addLayout(move_info_layout);
    main_form_layout->addLayout(move_layout);

    //category
    QComboBox* move_category = new QComboBox;
    move_category->setObjectName("atk1_movecategories_combobox");

    //populating it
    move_category->addItem("Physical");
    move_category->addItem("Special");

    move_info_layout->addWidget(move_category);

    //bp
    QSpinBox* move_bp = new QSpinBox;
    move_bp->setObjectName("atk1_movebp_spinbox");
    move_bp->setRange(1, 999);

    move_info_layout->addWidget(move_bp);

    //crit
    QLabel* crit_label = new QLabel(tr("Crit"));
    move_info_layout->addWidget(crit_label);

    QCheckBox* crit = new QCheckBox;
    crit->setObjectName("atk1_crit");
    move_info_layout->addWidget(crit);

    //z
    QLabel* z_label = new QLabel(tr("Z"));
    move_info_layout->addWidget(z_label);

    QCheckBox* z = new QCheckBox;
    z->setObjectName("atk1_z");
    move_info_layout->addWidget(z);

    //adding everything to the layout
    atk1_layout->addLayout(main_form_layout);

    //enclosing it in a tab
    tabs->addTab(atk1_groupbox, tr("Attack #1"));

    //setting signals
    connect(species, SIGNAL(currentIndexChanged(int)), this, SLOT(setSpecies1(int)));
    connect(forms, SIGNAL(currentIndexChanged(int)), this, SLOT(setForm1(int)));
    connect(moves, SIGNAL(currentIndexChanged(int)), this, SLOT(setMove1(int)));
    connect(move_category, SIGNAL(currentIndexChanged(int)), this, SLOT(setMoveCategory1(int)));

    //setting index 0
    species->setCurrentIndex(1); //setting it to 1 because the signal is currentindexCHANGED, i am so stupid lol
    species->setCurrentIndex(0);

    //same
    moves->setCurrentIndex(1);
    moves->setCurrentIndex(0);
}

void DefenseMoveWindow::createAtk2GroupBox() {
    //the main horizontal layout for this window
    QHBoxLayout* atk2_layout = new QHBoxLayout;

    //the groupbox in which we encapsulate this window
    atk2_groupbox = new QGroupBox(tr("Attacking Pokemon 2:"));
    atk2_groupbox->setLayout(atk2_layout);

    //---SPECIES & TYPES---//

    //the main layout for this entire section
    QVBoxLayout* species_types_layout = new QVBoxLayout;
    species_types_layout->setSpacing(1);
    species_types_layout->setAlignment(Qt::AlignVCenter);

    //---activation box---
    QHBoxLayout* activated_layout = new QHBoxLayout;

    QLabel* activated_label = new QLabel(tr("Activated"));
    activated_label->setAlignment(Qt::AlignCenter);
    activated_layout->addWidget(activated_label);

    QCheckBox* activated = new QCheckBox;
    activated->setObjectName("atk2_activated");
    activated_layout->addWidget(activated);

    species_types_layout->addLayout(activated_layout);

    //---sprite---
    QHBoxLayout* sprite_layout = new QHBoxLayout;

    QLabel* sprite = new QLabel;
    sprite->setObjectName("atk2_sprite");
    sprite->setAlignment(Qt::AlignCenter);
    sprite_layout->addWidget(sprite);

    species_types_layout->addLayout(sprite_layout);

    //some spacing
    species_types_layout->insertSpacing(1, 10);

    //---species---//
    //the layout just for the species
    QHBoxLayout* species_layout = new QHBoxLayout;
    species_types_layout->addLayout(species_layout);

    //the combobox for all the species
    QComboBox* species = new QComboBox;
    species->setObjectName("atk2_species_combobox");

    //loading it with all the species name
    auto species_buffer = ((MainWindow*)parentWidget())->getSpeciesNames();
    for( auto it = species_buffer.begin(); it < species_buffer.end(); it++ ) species->addItem(*it);

    //some resizing
    int species_width = species->minimumSizeHint().width();
    species->setMaximumWidth(species_width);
    //adding it to the layout
    species_layout->addWidget(species);

    //---forms---//
    //the layout just for the forms
    QHBoxLayout* forms_layout = new QHBoxLayout;
    species_types_layout->addLayout(forms_layout);

    //the combobox for all the species
    QComboBox* forms = new QComboBox;
    forms->setObjectName("atk2_forms_combobox");

    //some resizing
    int forms_width = forms->minimumSizeHint().width();
    forms->setMaximumWidth(forms_width);
    //adding it to the layout
    forms_layout->addWidget(forms);

    //---types---//
    //the layout for the types
    QHBoxLayout* types_layout = new QHBoxLayout;

    //the comboboxes for the types
    QComboBox* types1 = new QComboBox;
    QComboBox* types2 = new QComboBox;
    types1->setObjectName("atk2_type1_combobox");
    types2->setObjectName("atk2_type2_combobox");

    //loading it with all the types names
    auto types_buffer = ((MainWindow*)parentWidget())->getTypesNames();
    for( auto it = types_buffer.begin(); it < types_buffer.end(); it++ ) { types1->addItem(*it); types2->addItem(*it); }

    //resizing them
    int types_width = species->minimumSizeHint().width();
    types1->setMaximumWidth(types_width);
    types2->setMaximumWidth(types_width);

    //adding them to the layout
    types_layout->addWidget(types1);
    types_layout->addWidget(types2);
    species_types_layout->addLayout(types_layout);

    //adding everything to the window
    atk2_layout->addLayout(species_types_layout);

    //and then some spacing
    atk2_layout->insertSpacing(1, 35);

    //---MAIN FORM---//
    QVBoxLayout* main_form_layout = new QVBoxLayout;

    //---information section---///
    QFormLayout* form_layout = new QFormLayout;
    main_form_layout->addLayout(form_layout);

    //NATURE
    //the combobox for the nature
    QComboBox* natures = new QComboBox;
    natures->setObjectName("atk2_nature_combobox");

    //populating it
    auto natures_buffer = ((MainWindow*)parentWidget())->getNaturesNames();
    for( auto it = natures_buffer.begin(); it < natures_buffer.end(); it++ ) natures->addItem(*it);

    form_layout->addRow(tr("Nature:"), natures);

    //ABILITY
    //the combobox for the ability
    QComboBox* abilities = new QComboBox;
    abilities->setObjectName("atk2_abilities_combobox");

    //populating it
    auto abilities_buffer = ((MainWindow*)parentWidget())->getAbilitiesNames();
    for( auto it = abilities_buffer.begin(); it < abilities_buffer.end(); it++ ) abilities->addItem(*it);

    //getting the abilities width (to be used later)
    int abilities_width = abilities->minimumSizeHint().width();

    form_layout->addRow(tr("Ability:"), abilities);

    //ITEM
    //the combobox for the item
    QComboBox* items = new QComboBox;
    items->setObjectName("atk2_items_combobox");

    //populating it
    auto items_buffer = ((MainWindow*)parentWidget())->getItemsNames();
    for( auto it = items_buffer.begin(); it < items_buffer.end(); it++ ) items->addItem(*it);

    form_layout->addRow(tr("Item:"), items);

    natures->setMaximumWidth(abilities_width);
    items->setMaximumWidth(abilities_width);
    abilities->setMaximumWidth(abilities_width);

    //some spacing
    main_form_layout->insertSpacing(1, 10);

    //---modifiers---//
    QHBoxLayout* modifiers_layout = new QHBoxLayout;
    modifiers_layout->setAlignment(Qt::AlignLeft);
    modifiers_layout->setSpacing(10);

    //iv
    QLabel* iv_label = new QLabel(tr("Atk IV:"));
    iv_label->setObjectName("atk2_iv_label");
    modifiers_layout->addWidget(iv_label);

    QSpinBox* iv = new QSpinBox;
    iv->setObjectName("atk2_iv_spinbox");
    iv->setRange(0, 31);
    iv->setValue(31);
    modifiers_layout->addWidget(iv);

    //ev
    QLabel* ev_label = new QLabel(tr("Atk EV:"));
    ev_label->setObjectName("atk2_ev_label");
    modifiers_layout->addWidget(ev_label);

    QSpinBox* ev = new QSpinBox;
    ev->setRange(0, 252);
    ev->setObjectName("atk2_ev_spinbox");
    modifiers_layout->addWidget(ev);

    //modifier
    QLabel* modifier_label = new QLabel(tr("Atk Modifier:"));
    modifier_label->setObjectName("atk2_modifier_label");
    modifiers_layout->addWidget(modifier_label);

    QSpinBox* modifier = new QSpinBox;
    modifier->setObjectName("atk2_modifier_spinbox");
    modifier->setRange(-6, 6);
    modifiers_layout->addWidget(modifier);

    main_form_layout->addLayout(modifiers_layout);

    //---moves---//
    QVBoxLayout* move_layout = new QVBoxLayout;
    move_layout->setAlignment(Qt::AlignLeft);

    //label
    QHBoxLayout* move_name_layout = new QHBoxLayout;
    move_name_layout->setAlignment(Qt::AlignLeft);
    QLabel* move_name_label = new QLabel(tr("Move:"));
    move_name_layout->addWidget(move_name_label);

    //name
    //the combobox for the move
    QComboBox* moves = new QComboBox;
    moves->setObjectName("atk2_moves_combobox");

    //populating it
    auto moves_buffer = ((MainWindow*)parentWidget())->getMovesNames();
    for( auto it = moves_buffer.begin(); it < moves_buffer.end(); it++ ) moves->addItem(*it);

    move_name_layout->addWidget(moves);

    move_layout->addLayout(move_name_layout);

    //target
    QHBoxLayout* move_info_layout = new QHBoxLayout;

    QComboBox* target = new QComboBox;
    target->setObjectName("atk2_target_combobox");

    //populating it
    target->addItem(tr("Single Target"));
    target->addItem(tr("Double Target"));

    move_info_layout->addWidget(target);

    //type
    QComboBox* move_types = new QComboBox;
    move_types->setObjectName("atk2_movetypes_combobox");

    //populating it
    auto movetypes_buffer = ((MainWindow*)parentWidget())->getTypesNames();
    for( auto it = movetypes_buffer.begin(); it < movetypes_buffer.end(); it++ ) move_types->addItem(*it);

    move_info_layout->addWidget(move_types);

    move_layout->addLayout(move_info_layout);
    main_form_layout->addLayout(move_layout);

    //category
    QComboBox* move_category = new QComboBox;
    move_category->setObjectName("atk2_movecategories_combobox");

    //populating it
    move_category->addItem("Physical");
    move_category->addItem("Special");

    move_info_layout->addWidget(move_category);

    //bp
    QSpinBox* move_bp = new QSpinBox;
    move_bp->setObjectName("atk2_movebp_spinbox");
    move_bp->setRange(1, 999);

    move_info_layout->addWidget(move_bp);

    //crit
    QLabel* crit_label = new QLabel(tr("Crit"));
    move_info_layout->addWidget(crit_label);

    QCheckBox* crit = new QCheckBox;
    crit->setObjectName("atk2_crit");
    move_info_layout->addWidget(crit);

    //z
    QLabel* z_label = new QLabel(tr("Z"));
    move_info_layout->addWidget(z_label);

    QCheckBox* z = new QCheckBox;
    z->setObjectName("atk2_z");
    move_info_layout->addWidget(z);

    //adding everything to the layout
    atk2_layout->addLayout(main_form_layout);

    //enclosing it in a tab
    tabs->addTab(atk2_groupbox, tr("Attack #2"));

    //setting signals
    connect(species, SIGNAL(currentIndexChanged(int)), this, SLOT(setSpecies2(int)));
    connect(forms, SIGNAL(currentIndexChanged(int)), this, SLOT(setForm2(int)));
    connect(moves, SIGNAL(currentIndexChanged(int)), this, SLOT(setMove2(int)));
    connect(activated, SIGNAL(stateChanged(int)), this, SLOT(activateAtk2(int)));
    connect(move_category, SIGNAL(currentIndexChanged(int)), this, SLOT(setMoveCategory2(int)));


    //activating attack number 2
    activated->setChecked(true);

    //setting index 0
    species->setCurrentIndex(1); //setting it to 1 because the signal is currentindexCHANGED, i am so stupid lol
    species->setCurrentIndex(0);

    //same
    moves->setCurrentIndex(1);
    moves->setCurrentIndex(0);
}

void DefenseMoveWindow::createDefendingGroupBox() {
    defending_groupbox = new QGroupBox("Defending Pokemon Modifiers:");

    QHBoxLayout* defending_layout = new QHBoxLayout;
    defending_groupbox->setLayout(defending_layout);

    QLabel* modifier_label = new QLabel;
    modifier_label->setText(tr("Defense modifier"));
    defending_layout->addWidget(modifier_label);

    QSpinBox* def_modifier_spinbox = new QSpinBox;
    def_modifier_spinbox->setObjectName("defending_def_modifier");
    def_modifier_spinbox->setRange(-6, 6);
    defending_layout->addWidget(def_modifier_spinbox, Qt::AlignLeft);

    QLabel* spdef_modifier_label = new QLabel;
    spdef_modifier_label->setText(tr("Sp. Defense modifier"));
    defending_layout->addWidget(spdef_modifier_label);

    QSpinBox* spdef_modifier_spinbox = new QSpinBox;
    spdef_modifier_spinbox->setObjectName("defending_spdef_modifier");
    spdef_modifier_spinbox->setRange(-6, 6);
    defending_layout->addWidget(spdef_modifier_spinbox, Qt::AlignLeft);

    QLabel* hp_modifier_label = new QLabel;
    hp_modifier_label->setText(tr("HP percentage:"));
    defending_layout->addWidget(hp_modifier_label);

    QSpinBox* hp_modifier_spinbox = new QSpinBox;
    hp_modifier_spinbox->setObjectName("defending_hp_modifier");
    hp_modifier_spinbox->setRange(0, 100);
    hp_modifier_spinbox->setValue(100);
    hp_modifier_spinbox->setSuffix("%");
    defending_layout->addWidget(hp_modifier_spinbox, Qt::AlignLeft);

    QLabel* hits_modifier_label = new QLabel;
    hits_modifier_label->setText(tr("Number of hits:"));
    defending_layout->addWidget(hits_modifier_label);

    QSpinBox* hits_modifier_spinbox = new QSpinBox;
    hits_modifier_spinbox->setObjectName("defending_hits_modifier");
    hits_modifier_spinbox->setRange(2, 5);
    hits_modifier_spinbox->setSuffix("HKO");
    defending_layout->addWidget(hits_modifier_spinbox, Qt::AlignLeft);

    modifier_groupbox = new QGroupBox("Modifiers:");

    QHBoxLayout* modifier_layout = new QHBoxLayout;
    modifier_layout->setAlignment(Qt::AlignLeft);

    modifier_groupbox->setLayout(modifier_layout);

    //WEATHER
    QHBoxLayout* weather_layout = new QHBoxLayout;
    weather_layout->setAlignment(Qt::AlignLeft);

    QLabel* weather_label = new QLabel(tr("Weather:"));
    weather_layout->addWidget(weather_label);

    QComboBox* weather_combobox = new QComboBox;
    weather_combobox->setObjectName("weather_combobox");

    weather_combobox->addItem(tr("None"));
    weather_combobox->addItem(tr("Sun"));
    weather_combobox->addItem(tr("Rain"));
    weather_combobox->addItem(tr("Harsh Sunshine"));
    weather_combobox->addItem(tr("Heavy Rain"));
    weather_combobox->addItem(tr("Strong Winds"));

    weather_layout->addWidget(weather_combobox);

    modifier_layout->addLayout(weather_layout);

    //TERRAIN
    QHBoxLayout* terrain_layout = new QHBoxLayout;
    terrain_layout->setAlignment(Qt::AlignLeft);

    QLabel* terrain_label = new QLabel(tr("Terrain:"));
    terrain_layout->addWidget(terrain_label);

    QComboBox* terrain_combobox = new QComboBox;
    terrain_combobox->setObjectName("terrain_combobox");

    terrain_combobox->addItem(tr("None"));
    terrain_combobox->addItem(tr("Grassy"));
    terrain_combobox->addItem(tr("Electric"));
    terrain_combobox->addItem(tr("Psychich"));
    terrain_combobox->addItem(tr("Misty"));

    terrain_layout->addWidget(terrain_combobox);

    modifier_layout->addLayout(terrain_layout);
}

void DefenseMoveWindow::setMove1(int index) {
    Move selected_move((Moves)index);

    atk1_groupbox->findChild<QComboBox*>("atk1_movetypes_combobox")->setCurrentIndex(selected_move.getMoveType());

    QComboBox* target = atk1_groupbox->findChild<QComboBox*>("atk1_target_combobox");
    if( selected_move.isSpread() ) { target->setCurrentIndex(Move::Target::DOUBLE); target->setVisible(true); }
    else { target->setCurrentIndex(Move::Target::SINGLE); target->setVisible(false); }

    atk1_groupbox->findChild<QComboBox*>("atk1_movecategories_combobox")->setCurrentIndex(selected_move.getMoveCategory());
    atk1_groupbox->findChild<QSpinBox*>("atk1_movebp_spinbox")->setValue(selected_move.getBasePower());

    if( selected_move.isSignatureZ() ) atk1_groupbox->findChild<QCheckBox*>("atk1_z")->setEnabled(false);
    else atk1_groupbox->findChild<QCheckBox*>("atk1_z")->setEnabled(true);

    if( selected_move.getMoveCategory() == Move::Category::PHYSICAL ) {
        atk1_groupbox->findChild<QLabel*>("atk1_iv_label")->setText(tr("Atk IV"));
        atk1_groupbox->findChild<QLabel*>("atk1_ev_label")->setText(tr("Atk EV"));
        atk1_groupbox->findChild<QLabel*>("atk1_modifier_label")->setText(tr("Atk Modifier"));
    }

    else {
        atk1_groupbox->findChild<QLabel*>("atk1_iv_label")->setText(tr("Sp. Atk IV"));
        atk1_groupbox->findChild<QLabel*>("atk1_ev_label")->setText(tr("Sp. Atk EV"));
        atk1_groupbox->findChild<QLabel*>("atk1_modifier_label")->setText(tr("Sp. Atk Modifier"));
    }
}

void DefenseMoveWindow::setSpecies1(int index) {
    Pokemon selected_pokemon(index + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = atk1_groupbox->findChild<QLabel*>("atk1_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting correct types
    QComboBox* type1 = atk1_groupbox->findChild<QComboBox*>("atk1_type1_combobox");
    type1->setCurrentIndex(selected_pokemon.getTypes()[0][0]);

    QComboBox* type2 = atk1_groupbox->findChild<QComboBox*>("atk1_type2_combobox");
    type2->setCurrentIndex(selected_pokemon.getTypes()[0][1]);

    if( selected_pokemon.getTypes()[0][0] == selected_pokemon.getTypes()[0][1] ) { type2->setVisible(false); type2->setVisible(false); }
    else { type2->setVisible(true); type2->setVisible(true); }

    //setting correct form
    QComboBox* form = atk1_groupbox->findChild<QComboBox*>("atk1_forms_combobox");
    if( selected_pokemon.getFormesNumber() > 1 ) {
        form->clear();
        for(unsigned int i = 0; i < selected_pokemon.getFormesNumber(); i++) form->addItem(((MainWindow*)this->parentWidget())->retrieveFormName(index + 1, i));
        form->setCurrentIndex(0);
        form->setVisible(true);
    }

    else {
        form->setCurrentIndex(0);
        form->setVisible(false);
    }

    //setting correct ability
    QComboBox* ability = atk1_groupbox->findChild<QComboBox*>("atk1_abilities_combobox");
    ability->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0][0]);
}

void DefenseMoveWindow::setForm1(int index) {
    Pokemon selected_pokemon(atk1_groupbox->findChild<QComboBox*>("atk1_species_combobox")->currentIndex() + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path;
    bool load_result;

    if( index == 0 ) sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    else sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + "-" + QString::number(index) + ".png";

    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = atk1_groupbox->findChild<QLabel*>("atk1_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting correct types
    QComboBox* type1 = atk1_groupbox->findChild<QComboBox*>("atk1_type1_combobox");
    type1->setCurrentIndex(selected_pokemon.getTypes()[index][0]);

    QComboBox* type2 = atk1_groupbox->findChild<QComboBox*>("atk1_type2_combobox");
    type2->setCurrentIndex(selected_pokemon.getTypes()[index][1]);

    if( selected_pokemon.getTypes()[index][0] == selected_pokemon.getTypes()[index][1] ) { type2->setVisible(false); type2->setVisible(false); }
    else { type2->setVisible(true); type2->setVisible(true); }

    //setting correct ability
    QComboBox* ability = atk1_groupbox->findChild<QComboBox*>("atk1_abilities_combobox");
    ability->setCurrentIndex(selected_pokemon.getPossibleAbilities()[index][0]);
}

void DefenseMoveWindow::setForm2(int index) {
    Pokemon selected_pokemon(atk2_groupbox->findChild<QComboBox*>("atk2_species_combobox")->currentIndex() + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path;
    bool load_result;

    if( index == 0 ) sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    else sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + "-" + QString::number(index) + ".png";

    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = atk2_groupbox->findChild<QLabel*>("atk2_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting correct types
    QComboBox* type1 = atk2_groupbox->findChild<QComboBox*>("atk2_type1_combobox");
    type1->setCurrentIndex(selected_pokemon.getTypes()[index][0]);

    QComboBox* type2 = atk2_groupbox->findChild<QComboBox*>("atk2_type2_combobox");
    type2->setCurrentIndex(selected_pokemon.getTypes()[index][1]);

    if( selected_pokemon.getTypes()[index][0] == selected_pokemon.getTypes()[index][1] ) { type2->setVisible(false); type2->setVisible(false); }
    else { type2->setVisible(true); type2->setVisible(true); }

    //setting correct ability
    QComboBox* ability = atk2_groupbox->findChild<QComboBox*>("atk2_abilities_combobox");
    ability->setCurrentIndex(selected_pokemon.getPossibleAbilities()[index][0]);
}

void DefenseMoveWindow::setMove2(int index) {
    Move selected_move((Moves)index);

    atk2_groupbox->findChild<QComboBox*>("atk2_movetypes_combobox")->setCurrentIndex(selected_move.getMoveType());

    QComboBox* target = atk2_groupbox->findChild<QComboBox*>("atk2_target_combobox");
    if( selected_move.isSpread() ) { target->setCurrentIndex(Move::Target::DOUBLE); target->setVisible(true); }
    else { target->setCurrentIndex(Move::Target::SINGLE); target->setVisible(false); }

    atk2_groupbox->findChild<QComboBox*>("atk2_movecategories_combobox")->setCurrentIndex(selected_move.getMoveCategory());
    atk2_groupbox->findChild<QSpinBox*>("atk2_movebp_spinbox")->setValue(selected_move.getBasePower());

    if( selected_move.isSignatureZ() ) atk2_groupbox->findChild<QCheckBox*>("atk2_z")->setEnabled(false);
    else atk2_groupbox->findChild<QCheckBox*>("atk2_z")->setEnabled(true);

    if( selected_move.getMoveCategory() == Move::Category::PHYSICAL ) {
        atk2_groupbox->findChild<QLabel*>("atk2_iv_label")->setText(tr("Atk IV"));
        atk2_groupbox->findChild<QLabel*>("atk2_ev_label")->setText(tr("Atk EV"));
        atk2_groupbox->findChild<QLabel*>("atk2_modifier_label")->setText(tr("Atk Modifier"));
    }

    else {
        atk2_groupbox->findChild<QLabel*>("atk2_iv_label")->setText(tr("Sp. Atk IV"));
        atk2_groupbox->findChild<QLabel*>("atk2_ev_label")->setText(tr("Sp. Atk EV"));
        atk2_groupbox->findChild<QLabel*>("atk2_modifier_label")->setText(tr("Sp. Atk Modifier"));
    }
}

void DefenseMoveWindow::setSpecies2(int index) {
    Pokemon selected_pokemon(index + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = atk2_groupbox->findChild<QLabel*>("atk2_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting correct types
    QComboBox* type1 = atk2_groupbox->findChild<QComboBox*>("atk2_type1_combobox");
    type1->setCurrentIndex(selected_pokemon.getTypes()[0][0]);

    QComboBox* type2 = atk2_groupbox->findChild<QComboBox*>("atk2_type2_combobox");
    type2->setCurrentIndex(selected_pokemon.getTypes()[0][1]);

    if( selected_pokemon.getTypes()[0][0] == selected_pokemon.getTypes()[0][1] ) { type2->setVisible(false); type2->setVisible(false); }
    else { type2->setVisible(true); type2->setVisible(true); }

    //setting correct form
    QComboBox* form = atk2_groupbox->findChild<QComboBox*>("atk2_forms_combobox");
    if( selected_pokemon.getFormesNumber() > 1 ) {
        form->clear();
        for(unsigned int i = 0; i < selected_pokemon.getFormesNumber(); i++) form->addItem(((MainWindow*)this->parentWidget())->retrieveFormName(index + 1, i));
        form->setCurrentIndex(0);
        form->setVisible(true);
    }

    else {
        form->setCurrentIndex(0);
        form->setVisible(false);
    }

    //setting correct ability
    QComboBox* ability = atk2_groupbox->findChild<QComboBox*>("atk2_abilities_combobox");
    ability->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0][0]);
}

void DefenseMoveWindow::activateAtk2(int state) {
    bool value;
    if( state == Qt::Checked ) value = true;
    else value = false;

    atk2_groupbox->findChild<QComboBox*>("atk2_species_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_forms_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_type1_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_type2_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_nature_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_abilities_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_items_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QSpinBox*>("atk2_iv_spinbox")->setEnabled(value);
    atk2_groupbox->findChild<QSpinBox*>("atk2_ev_spinbox")->setEnabled(value);
    atk2_groupbox->findChild<QSpinBox*>("atk2_modifier_spinbox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_moves_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_movetypes_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_target_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QComboBox*>("atk2_movecategories_combobox")->setEnabled(value);
    atk2_groupbox->findChild<QSpinBox*>("atk2_movebp_spinbox")->setEnabled(value);
    atk2_groupbox->findChild<QCheckBox*>("atk2_crit")->setEnabled(value);
    atk2_groupbox->findChild<QCheckBox*>("atk2_z")->setEnabled(value);
}

void DefenseMoveWindow::solveMove(void) {
    Pokemon attacking1(atk1_groupbox->findChild<QComboBox*>("atk1_species_combobox")->currentIndex()+1);
    attacking1.setForm(atk1_groupbox->findChild<QComboBox*>("atk1_forms_combobox")->currentIndex());

    attacking1.setType(0, (Type)atk1_groupbox->findChild<QComboBox*>("atk1_type1_combobox")->currentIndex());
    attacking1.setType(1, (Type)atk1_groupbox->findChild<QComboBox*>("atk1_type2_combobox")->currentIndex());
    attacking1.setNature((Stats::Nature)atk1_groupbox->findChild<QComboBox*>("atk1_nature_combobox")->currentIndex());
    attacking1.setAbility((Ability)atk1_groupbox->findChild<QComboBox*>("atk1_abilities_combobox")->currentIndex());
    attacking1.setItem(Item(atk1_groupbox->findChild<QComboBox*>("atk1_items_combobox")->currentIndex()));

    Move attacking1_move((Moves)atk1_groupbox->findChild<QComboBox*>("atk1_moves_combobox")->currentIndex());
    if( atk1_groupbox->findChild<QComboBox*>("atk1_target_combobox")->currentIndex() == Move::Target::SINGLE ) attacking1_move.setTarget(Move::Target::SINGLE);
    else attacking1_move.setTarget(Move::Target::DOUBLE);
    attacking1_move.setMoveType((Type)atk1_groupbox->findChild<QComboBox*>("atk1_movetypes_combobox")->currentIndex());
    attacking1_move.setMoveCategory((Move::Category)atk1_groupbox->findChild<QComboBox*>("atk1_movecategories_combobox")->currentIndex());
    attacking1_move.setBasePower(atk1_groupbox->findChild<QSpinBox*>("atk1_movebp_spinbox")->value());

    if( atk1_groupbox->findChild<QCheckBox*>("atk1_crit")->checkState() == Qt::Checked ) attacking1_move.setCrit(true);
    else if(atk1_groupbox->findChild<QCheckBox*>("atk1_crit")->checkState() == Qt::Unchecked ) attacking1_move.setCrit(false);

    if( atk1_groupbox->findChild<QCheckBox*>("atk1_z")->checkState() == Qt::Checked ) attacking1_move.setZ(true);
    else if( atk1_groupbox->findChild<QCheckBox*>("atk1_z")->checkState() == Qt::Unchecked ) attacking1_move.setZ(false);

    attacking1_move.setWeather((Move::Weather)modifier_groupbox->findChild<QComboBox*>("weather_combobox")->currentIndex());
    attacking1_move.setTerrain((Move::Terrain)modifier_groupbox->findChild<QComboBox*>("terrain_combobox")->currentIndex());

    //now setting pokemon 1 iv/ev/modifier
    Stats::Stat stat;
    if( attacking1_move.getMoveCategory() == Move::Category::PHYSICAL ) stat = Stats::ATK;
    else stat = Stats::SPATK;

    attacking1.setIV(stat, atk1_groupbox->findChild<QSpinBox*>("atk1_iv_spinbox")->value());
    attacking1.setEV(stat, atk1_groupbox->findChild<QSpinBox*>("atk1_ev_spinbox")->value());
    attacking1.setModifier(stat, atk1_groupbox->findChild<QSpinBox*>("atk1_modifier_spinbox")->value());

    Pokemon attacking2(atk2_groupbox->findChild<QComboBox*>("atk2_species_combobox")->currentIndex()+1);
    attacking2.setForm(atk2_groupbox->findChild<QComboBox*>("atk2_forms_combobox")->currentIndex());

    attacking2.setType(0, (Type)atk2_groupbox->findChild<QComboBox*>("atk2_type1_combobox")->currentIndex());
    attacking2.setType(1, (Type)atk2_groupbox->findChild<QComboBox*>("atk2_type2_combobox")->currentIndex());
    attacking2.setNature((Stats::Nature)atk2_groupbox->findChild<QComboBox*>("atk2_nature_combobox")->currentIndex());
    attacking2.setAbility((Ability)atk2_groupbox->findChild<QComboBox*>("atk2_abilities_combobox")->currentIndex());
    attacking2.setItem(Item(atk2_groupbox->findChild<QComboBox*>("atk2_items_combobox")->currentIndex()));

    Move attacking2_move((Moves)atk2_groupbox->findChild<QComboBox*>("atk2_moves_combobox")->currentIndex());
    if( atk2_groupbox->findChild<QComboBox*>("atk2_target_combobox")->currentIndex() == Move::Target::SINGLE ) attacking2_move.setTarget(Move::Target::SINGLE);
    else attacking2_move.setTarget(Move::Target::DOUBLE);
    attacking2_move.setMoveType((Type)atk2_groupbox->findChild<QComboBox*>("atk2_movetypes_combobox")->currentIndex());
    attacking2_move.setMoveCategory((Move::Category)atk2_groupbox->findChild<QComboBox*>("atk2_movecategories_combobox")->currentIndex());
    attacking2_move.setBasePower(atk2_groupbox->findChild<QSpinBox*>("atk2_movebp_spinbox")->value());

    if( atk2_groupbox->findChild<QCheckBox*>("atk2_crit")->checkState() == Qt::Checked ) attacking2_move.setCrit(true);
    else if(atk2_groupbox->findChild<QCheckBox*>("atk2_crit")->checkState() == Qt::Unchecked ) attacking2_move.setCrit(false);

    if( atk2_groupbox->findChild<QCheckBox*>("atk2_z")->checkState() == Qt::Checked ) attacking2_move.setZ(true);
    else if( atk2_groupbox->findChild<QCheckBox*>("atk2_z")->checkState() == Qt::Unchecked ) attacking2_move.setZ(false);

    attacking2_move.setWeather((Move::Weather)modifier_groupbox->findChild<QComboBox*>("weather_combobox")->currentIndex());
    attacking2_move.setTerrain((Move::Terrain)modifier_groupbox->findChild<QComboBox*>("terrain_combobox")->currentIndex());

    //now setting pokemon 2 iv/ev/modifier
    Stats::Stat stat2;
    if( attacking2_move.getMoveCategory() == Move::Category::PHYSICAL ) stat2 = Stats::ATK;
    else stat2 = Stats::SPATK;

    attacking2.setIV(stat2, atk2_groupbox->findChild<QSpinBox*>("atk2_iv_spinbox")->value());
    attacking2.setEV(stat2, atk2_groupbox->findChild<QSpinBox*>("atk2_ev_spinbox")->value());
    attacking2.setModifier(stat2, atk2_groupbox->findChild<QSpinBox*>("atk2_modifier_spinbox")->value());

    Turn turn;
    turn.addMove(attacking1, attacking1_move);
    if( atk2_groupbox->findChild<QCheckBox*>("atk2_activated")->checkState() == Qt::Checked ) turn.addMove(attacking2, attacking2_move);
    turn.setHits(defending_groupbox->findChild<QSpinBox*>("defending_hits_modifier")->value()-1);

    defense_modifier def_mod = std::make_tuple(defending_groupbox->findChild<QSpinBox*>("defending_hp_modifier")->value(), defending_groupbox->findChild<QSpinBox*>("defending_def_modifier")->value(), defending_groupbox->findChild<QSpinBox*>("defending_spdef_modifier")->value());

    ((MainWindow*)parentWidget())->addDefenseTurn(turn, def_mod);
}

void DefenseMoveWindow::setAsBlank() {
    //atk1
    atk1_groupbox->findChild<QComboBox*>("atk1_species_combobox")->setCurrentIndex(0);
    //atk1_groupbox->findChild<QComboBox*>("atk1_type1_combobox")->setCurrentIndex(0);
    //atk1_groupbox->findChild<QComboBox*>("atk1_type2_combobox")->setCurrentIndex(0);
    atk1_groupbox->findChild<QComboBox*>("atk1_forms_combobox")->setCurrentIndex(0);
    atk1_groupbox->findChild<QComboBox*>("atk1_nature_combobox")->setCurrentIndex(0);
    atk1_groupbox->findChild<QComboBox*>("atk1_items_combobox")->setCurrentIndex(0);
    //atk1_groupbox->findChild<QComboBox*>("atk1_abilities_combobox")->setCurrentIndex(0);

    atk1_groupbox->findChild<QSpinBox*>("atk1_iv_spinbox")->setValue(31);
    atk1_groupbox->findChild<QSpinBox*>("atk1_ev_spinbox")->setValue(0);
    atk1_groupbox->findChild<QSpinBox*>("atk1_modifier_spinbox")->setValue(0);

    atk1_groupbox->findChild<QComboBox*>("atk1_moves_combobox")->setCurrentIndex(0);
    atk1_groupbox->findChild<QCheckBox*>("atk1_crit")->setChecked(false);
    atk1_groupbox->findChild<QCheckBox*>("atk1_z")->setChecked(false);

    //atk2
    atk2_groupbox->findChild<QCheckBox*>("atk2_activated")->setChecked(true);
    //atk2_groupbox->findChild<QComboBox*>("atk2_type1_combobox")->setCurrentIndex(0);
    //atk2_groupbox->findChild<QComboBox*>("atk2_type2_combobox")->setCurrentIndex(0);
    atk2_groupbox->findChild<QComboBox*>("atk2_species_combobox")->setCurrentIndex(0);
    atk2_groupbox->findChild<QComboBox*>("atk2_forms_combobox")->setCurrentIndex(0);
    atk2_groupbox->findChild<QComboBox*>("atk2_nature_combobox")->setCurrentIndex(0);
    atk2_groupbox->findChild<QComboBox*>("atk2_items_combobox")->setCurrentIndex(0);
    //atk2_groupbox->findChild<QComboBox*>("atk2_abilities_combobox")->setCurrentIndex(0);

    atk2_groupbox->findChild<QSpinBox*>("atk2_iv_spinbox")->setValue(31);
    atk2_groupbox->findChild<QSpinBox*>("atk2_ev_spinbox")->setValue(0);
    atk2_groupbox->findChild<QSpinBox*>("atk2_modifier_spinbox")->setValue(0);

    atk2_groupbox->findChild<QComboBox*>("atk2_moves_combobox")->setCurrentIndex(0);
    atk2_groupbox->findChild<QCheckBox*>("atk2_crit")->setChecked(false);
    atk2_groupbox->findChild<QCheckBox*>("atk2_z")->setChecked(false);

    modifier_groupbox->findChild<QComboBox*>("weather_combobox")->setCurrentIndex(0);
    modifier_groupbox->findChild<QComboBox*>("terrain_combobox")->setCurrentIndex(0);

    defending_groupbox->findChild<QSpinBox*>("defending_def_modifier")->setValue(0);
    defending_groupbox->findChild<QSpinBox*>("defending_spdef_modifier")->setValue(0);
    defending_groupbox->findChild<QSpinBox*>("defending_hp_modifier")->setValue(100);
    defending_groupbox->findChild<QSpinBox*>("defending_hits_modifier")->setValue(0);

    tabs->setCurrentIndex(0);
}

void DefenseMoveWindow::setAsTurn(const Turn &theTurn, const defense_modifier &theDefenseModifier) {
    //atk1
    atk1_groupbox->findChild<QComboBox*>("atk1_species_combobox")->setCurrentIndex(theTurn.getMoves()[0].first.getPokedexNumber()-1);
    atk1_groupbox->findChild<QComboBox*>("atk1_type1_combobox")->setCurrentIndex(theTurn.getMoves()[0].first.getTypes()[theTurn.getMoves()[0].first.getForm()][0]);
    atk1_groupbox->findChild<QComboBox*>("atk1_type2_combobox")->setCurrentIndex(theTurn.getMoves()[0].first.getTypes()[theTurn.getMoves()[0].first.getForm()][1]);
    atk1_groupbox->findChild<QComboBox*>("atk1_forms_combobox")->setCurrentIndex(theTurn.getMoves()[0].first.getForm());
    atk1_groupbox->findChild<QComboBox*>("atk1_nature_combobox")->setCurrentIndex(theTurn.getMoves()[0].first.getNature());
    atk1_groupbox->findChild<QComboBox*>("atk1_items_combobox")->setCurrentIndex(theTurn.getMoves()[0].first.getItem().getIndex());
    atk1_groupbox->findChild<QComboBox*>("atk1_abilities_combobox")->setCurrentIndex(theTurn.getMoves()[0].first.getAbility());

    atk1_groupbox->findChild<QComboBox*>("atk1_moves_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getMoveIndex());
    atk1_groupbox->findChild<QComboBox*>("atk1_target_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getTarget());
    atk1_groupbox->findChild<QComboBox*>("atk1_movetypes_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getMoveType());
    atk1_groupbox->findChild<QComboBox*>("atk1_movecategories_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getMoveCategory());
    atk1_groupbox->findChild<QSpinBox*>("atk1_movebp_spinbox")->setValue(theTurn.getMoves()[0].second.getBasePower());

    atk1_groupbox->findChild<QCheckBox*>("atk1_crit")->setChecked(theTurn.getMoves()[0].second.isCrit());
    atk1_groupbox->findChild<QCheckBox*>("atk1_z")->setChecked(theTurn.getMoves()[0].second.isZ());

    Stats::Stat stat;
    if( theTurn.getMoves()[0].second.getMoveCategory() == Move::Category::SPECIAL ) stat = Stats::SPATK;
    else stat = Stats::ATK;

    atk1_groupbox->findChild<QSpinBox*>("atk1_iv_spinbox")->setValue(theTurn.getMoves()[0].first.getIV(stat));
    atk1_groupbox->findChild<QSpinBox*>("atk1_ev_spinbox")->setValue(theTurn.getMoves()[0].first.getEV(stat));
    atk1_groupbox->findChild<QSpinBox*>("atk1_modifier_spinbox")->setValue(theTurn.getMoves()[0].first.getModifier(stat));

    if(theTurn.getMoveNum() > 1) {
        //atk2
        atk2_groupbox->findChild<QComboBox*>("atk2_species_combobox")->setCurrentIndex(theTurn.getMoves()[1].first.getPokedexNumber()-1);
        atk2_groupbox->findChild<QComboBox*>("atk2_type1_combobox")->setCurrentIndex(theTurn.getMoves()[1].first.getTypes()[theTurn.getMoves()[1].first.getForm()][0]);
        atk2_groupbox->findChild<QComboBox*>("atk2_type2_combobox")->setCurrentIndex(theTurn.getMoves()[1].first.getTypes()[theTurn.getMoves()[1].first.getForm()][1]);
        atk2_groupbox->findChild<QComboBox*>("atk2_forms_combobox")->setCurrentIndex(theTurn.getMoves()[1].first.getForm());
        atk2_groupbox->findChild<QComboBox*>("atk2_nature_combobox")->setCurrentIndex(theTurn.getMoves()[1].first.getNature());
        atk2_groupbox->findChild<QComboBox*>("atk2_items_combobox")->setCurrentIndex(theTurn.getMoves()[1].first.getItem().getIndex());
        atk2_groupbox->findChild<QComboBox*>("atk2_abilities_combobox")->setCurrentIndex(theTurn.getMoves()[1].first.getAbility());

        atk2_groupbox->findChild<QComboBox*>("atk2_moves_combobox")->setCurrentIndex(theTurn.getMoves()[1].second.getMoveIndex());
        atk2_groupbox->findChild<QComboBox*>("atk2_target_combobox")->setCurrentIndex(theTurn.getMoves()[1].second.getTarget());
        atk2_groupbox->findChild<QComboBox*>("atk2_movetypes_combobox")->setCurrentIndex(theTurn.getMoves()[1].second.getMoveType());
        atk2_groupbox->findChild<QComboBox*>("atk2_movecategories_combobox")->setCurrentIndex(theTurn.getMoves()[1].second.getMoveCategory());
        atk2_groupbox->findChild<QSpinBox*>("atk2_movebp_spinbox")->setValue(theTurn.getMoves()[1].second.getBasePower());

        atk2_groupbox->findChild<QCheckBox*>("atk2_crit")->setChecked(theTurn.getMoves()[1].second.isCrit());
        atk2_groupbox->findChild<QCheckBox*>("atk2_z")->setChecked(theTurn.getMoves()[1].second.isZ());

        Stats::Stat stat2;
        if( theTurn.getMoves()[1].second.getMoveCategory() == Move::Category::SPECIAL ) stat2 = Stats::SPATK;
        else stat2 = Stats::ATK;

        atk2_groupbox->findChild<QSpinBox*>("atk2_iv_spinbox")->setValue(theTurn.getMoves()[1].first.getIV(stat2));
        atk2_groupbox->findChild<QSpinBox*>("atk2_ev_spinbox")->setValue(theTurn.getMoves()[1].first.getEV(stat2));
        atk2_groupbox->findChild<QSpinBox*>("atk2_modifier_spinbox")->setValue(theTurn.getMoves()[1].first.getModifier(stat2));
    }

    else atk2_groupbox->findChild<QCheckBox*>("atk2_activated")->setChecked(false);

    modifier_groupbox->findChild<QComboBox*>("weather_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getWeather());
    modifier_groupbox->findChild<QComboBox*>("terrain_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getTerrain());

    defending_groupbox->findChild<QSpinBox*>("defending_def_modifier")->setValue(std::get<1>(theDefenseModifier));
    defending_groupbox->findChild<QSpinBox*>("defending_spdef_modifier")->setValue(std::get<2>(theDefenseModifier));
    defending_groupbox->findChild<QSpinBox*>("defending_hp_modifier")->setValue(std::get<0>(theDefenseModifier));
    defending_groupbox->findChild<QSpinBox*>("defending_hits_modifier")->setValue(theTurn.getHits()+1);

    tabs->setCurrentIndex(0);
}

QString DefenseMoveWindow::retrieveFormName(const int species, const int form) {
    //this function is written with the help of Kaphotics here: https://github.com/kwsch/PKHeX/issues/2259
    if( form == 0 ) return "Base";

    /*const int MAX_SPECIES = 804;
    int index = MAX_SPECIES;
    for (int i = 1; i <= MAX_SPECIES; i++) {
        if (i == species) return forms_names[index + form - 1];
        Pokemon selected_pokemon(i);
        index += selected_pokemon.getFormesNumber() - 1;
    }*/

    else return "Form " + QString::number(form);
}

void DefenseMoveWindow::setMoveCategory1(int index) {
    if( index == Move::Category::PHYSICAL ) {
        atk1_groupbox->findChild<QLabel*>("atk1_iv_label")->setText(tr("Atk IV"));
        atk1_groupbox->findChild<QLabel*>("atk1_ev_label")->setText(tr("Atk EV"));
        atk1_groupbox->findChild<QLabel*>("atk1_modifier_label")->setText(tr("Atk Modifier"));
    }

    else {
        atk1_groupbox->findChild<QLabel*>("atk1_iv_label")->setText(tr("Sp. Atk IV"));
        atk1_groupbox->findChild<QLabel*>("atk1_ev_label")->setText(tr("Sp. Atk EV"));
        atk1_groupbox->findChild<QLabel*>("atk1_modifier_label")->setText(tr("Sp. Atk Modifier"));
    }
}

void DefenseMoveWindow::setMoveCategory2(int index) {
    if( index == Move::Category::PHYSICAL ) {
        atk2_groupbox->findChild<QLabel*>("atk2_iv_label")->setText(tr("Atk IV"));
        atk2_groupbox->findChild<QLabel*>("atk2_ev_label")->setText(tr("Atk EV"));
        atk2_groupbox->findChild<QLabel*>("atk2_modifier_label")->setText(tr("Atk Modifier"));
    }

    else {
        atk2_groupbox->findChild<QLabel*>("atk2_iv_label")->setText(tr("Sp. Atk IV"));
        atk2_groupbox->findChild<QLabel*>("atk2_ev_label")->setText(tr("Sp. Atk EV"));
        atk2_groupbox->findChild<QLabel*>("atk2_modifier_label")->setText(tr("Sp. Atk Modifier"));
    }
}
