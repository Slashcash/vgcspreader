#include "attackmovewindow.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QDebug>

#include "mainwindow.hpp"

//a lot of this class is basicly recycled from defensemovewindow.cpp, maybe they should inherit from a common source

AttackMoveWindow::AttackMoveWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    edit_mode = false;

    QVBoxLayout* main_layout = new QVBoxLayout;
    setLayout(main_layout);

    createDefendingPokemonGroupbox();
    createModifierGroupbox();
    createMoveGroupbox();

    main_layout->addWidget(defending_pokemon_groupbox);
    main_layout->addWidget(move_groupbox);
    main_layout->addWidget(move_modifier_groupbox);
    main_layout->addWidget(atk_modifier_groupbox);

    bottom_button_box = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    main_layout->addWidget(bottom_button_box, Qt::AlignRight);

    //SIGNALS
    connect(bottom_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bottom_button_box, SIGNAL(accepted()), this, SLOT(solveMove(void)));
    connect(bottom_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void AttackMoveWindow::createDefendingPokemonGroupbox() {
    //the main horizontal layout for this window
    QHBoxLayout* def_layout = new QHBoxLayout;

    defending_pokemon_groupbox = new QGroupBox(tr("Defending Pokemon:"));
    defending_pokemon_groupbox->setLayout(def_layout);

    //---SPECIES & TYPES---//

    //---species---//
    //the main layout for this entire section
    QVBoxLayout* species_types_layout = new QVBoxLayout;
    species_types_layout->setSpacing(1);
    species_types_layout->setAlignment(Qt::AlignVCenter);

    //---sprite---
    QHBoxLayout* sprite_layout = new QHBoxLayout;

    QLabel* sprite = new QLabel;
    sprite->setObjectName("def_sprite");
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
    species->setObjectName("def_species_combobox");

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
    forms->setObjectName("def_forms_combobox");

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
    types1->setObjectName("def_type1_combobox");
    types2->setObjectName("def_type2_combobox");

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
    def_layout->addLayout(species_types_layout);

    //and then some spacing
    def_layout->insertSpacing(1, 35);

    //---MAIN FORM---//
    QVBoxLayout* main_form_layout = new QVBoxLayout;

    //---information section---///
    QFormLayout* form_layout = new QFormLayout;
    main_form_layout->addLayout(form_layout);

    //NATURE
    //the combobox for the nature
    QComboBox* natures = new QComboBox;
    natures->setObjectName("def_nature_combobox");

    //populating it
    auto natures_buffer = ((MainWindow*)parentWidget())->getNaturesNames();
    for( auto it = natures_buffer.begin(); it < natures_buffer.end(); it++ ) natures->addItem(*it);

    form_layout->addRow(tr("Nature:"), natures);

    //ABILITY
    //the combobox for the ability
    QComboBox* abilities = new QComboBox;
    abilities->setObjectName("def_abilities_combobox");

    //populating it
    auto abilities_buffer = ((MainWindow*)parentWidget())->getAbilitiesNames();
    for( auto it = abilities_buffer.begin(); it < abilities_buffer.end(); it++ ) abilities->addItem(*it);

    //getting the abilities width (to be used later)
    int abilities_width = abilities->minimumSizeHint().width();

    form_layout->addRow(tr("Ability:"), abilities);

    //ITEM
    //the combobox for the item
    QComboBox* items = new QComboBox;
    items->setObjectName("def_items_combobox");

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
    QHBoxLayout* modifiers_hp_layout = new QHBoxLayout;
    modifiers_hp_layout->setAlignment(Qt::AlignLeft);
    modifiers_hp_layout->setSpacing(10);

    //hp iv
    QLabel* hp_iv_label = new QLabel(tr("HP IV:"));
    hp_iv_label->setObjectName("hp_iv_label");
    modifiers_hp_layout->addWidget(hp_iv_label);

    QSpinBox* hp_iv = new QSpinBox;
    hp_iv->setObjectName("hp_iv_spinbox");
    hp_iv->setRange(0, 31);
    hp_iv->setValue(31);
    modifiers_hp_layout->addWidget(hp_iv);

    //hp ev
    QLabel* hp_ev_label = new QLabel(tr("HP EV:"));
    hp_ev_label->setObjectName("hp_ev_label");
    modifiers_hp_layout->addWidget(hp_ev_label);

    QSpinBox* hp_ev = new QSpinBox;
    hp_ev->setObjectName("hp_ev_spinbox");
    hp_ev->setRange(0, 252);
    modifiers_hp_layout->addWidget(hp_ev);

    //hp perc
    QLabel* hp_perc_label = new QLabel(tr("HP percentage:"));
    hp_perc_label->setObjectName("hp_perc_label");
    modifiers_hp_layout->addWidget(hp_perc_label);

    QSpinBox* hp_perc = new QSpinBox;
    hp_perc->setObjectName("hp_perc_spinbox");
    hp_perc->setRange(0, 100);
    hp_perc->setValue(100);
    hp_perc->setSuffix("%");
    modifiers_hp_layout->addWidget(hp_perc);

    main_form_layout->addLayout(modifiers_hp_layout);

    QHBoxLayout* modifiers_layout = new QHBoxLayout;
    modifiers_layout->setAlignment(Qt::AlignLeft);
    modifiers_layout->setSpacing(10);

    //iv
    QLabel* iv_label = new QLabel(tr("Def IV:"));
    iv_label->setObjectName("def_iv_label");
    modifiers_layout->addWidget(iv_label);

    QSpinBox* iv = new QSpinBox;
    iv->setObjectName("def_iv_spinbox");
    iv->setRange(0, 31);
    iv->setValue(31);
    modifiers_layout->addWidget(iv);

    //ev
    QLabel* ev_label = new QLabel(tr("Def EV:"));
    ev_label->setObjectName("def_ev_label");
    modifiers_layout->addWidget(ev_label);

    QSpinBox* ev = new QSpinBox;
    ev->setRange(0, 252);
    ev->setObjectName("def_ev_spinbox");
    modifiers_layout->addWidget(ev);

    //modifier
    QLabel* modifier_label = new QLabel(tr("Def Modifier:"));
    modifier_label->setObjectName("def_modifier_label");
    modifiers_layout->addWidget(modifier_label);

    QSpinBox* modifier = new QSpinBox;
    modifier->setObjectName("def_modifier_spinbox");
    modifier->setRange(-6, 6);
    modifiers_layout->addWidget(modifier);

    main_form_layout->addLayout(modifiers_layout);

    def_layout->addLayout(main_form_layout);

    //setting signals
    connect(species, SIGNAL(currentIndexChanged(int)), this, SLOT(setSpecies(int)));
    connect(forms, SIGNAL(currentIndexChanged(int)), this, SLOT(setForm(int)));

    //setting index 0
    species->setCurrentIndex(1); //setting it to 1 because the signal is currentindexCHANGED, i am so stupid lol
    species->setCurrentIndex(0);
}

void AttackMoveWindow::createMoveGroupbox() {
    //---moves---//
    QVBoxLayout* move_layout = new QVBoxLayout;
    move_layout->setAlignment(Qt::AlignLeft);

    move_groupbox = new QGroupBox(tr("Move:"));
    move_groupbox->setLayout(move_layout);

    //label
    QHBoxLayout* move_name_layout = new QHBoxLayout;
    move_name_layout->setAlignment(Qt::AlignLeft);
    QLabel* move_name_label = new QLabel(tr("Move:"));
    move_name_layout->addWidget(move_name_label);

    //name
    //the combobox for the move
    QComboBox* moves = new QComboBox;
    moves->setObjectName("moves_combobox");

    //populating it
    auto moves_buffer = ((MainWindow*)parentWidget())->getMovesNames();
    for( auto it = moves_buffer.begin(); it < moves_buffer.end(); it++ ) moves->addItem(*it);

    move_name_layout->addWidget(moves);

    move_layout->addLayout(move_name_layout);

    //target
    QHBoxLayout* move_info_layout = new QHBoxLayout;

    QComboBox* target = new QComboBox;
    target->setObjectName("target_combobox");

    //populating it
    target->addItem(tr("Single Target"));
    target->addItem(tr("Double Target"));

    move_info_layout->addWidget(target);

    //type
    QComboBox* move_types = new QComboBox;
    move_types->setObjectName("movetypes_combobox");

    //populating it
    auto movetypes_buffer = ((MainWindow*)parentWidget())->getTypesNames();
    for( auto it = movetypes_buffer.begin(); it < movetypes_buffer.end(); it++ ) move_types->addItem(*it);

    move_info_layout->addWidget(move_types);

    move_layout->addLayout(move_info_layout);
    //main_form_layout->addLayout(move_layout);

    //category
    QComboBox* move_category = new QComboBox;
    move_category->setObjectName("movecategories_combobox");

    //populating it
    move_category->addItem("Physical");
    move_category->addItem("Special");

    move_info_layout->addWidget(move_category);

    //bp
    QSpinBox* move_bp = new QSpinBox;
    move_bp->setObjectName("movebp_spinbox");
    move_bp->setRange(1, 999);

    move_info_layout->addWidget(move_bp);

    //crit
    QLabel* crit_label = new QLabel(tr("Crit"));
    move_info_layout->addWidget(crit_label);

    QCheckBox* crit = new QCheckBox;
    crit->setObjectName("crit");
    move_info_layout->addWidget(crit);

    //z
    QLabel* z_label = new QLabel(tr("Z"));
    move_info_layout->addWidget(z_label);

    QCheckBox* z = new QCheckBox;
    z->setObjectName("z");
    move_info_layout->addWidget(z);

    //setting signals
    connect(moves, SIGNAL(currentIndexChanged(int)), this, SLOT(setMove(int)));

    //setting index 0
    moves->setCurrentIndex(1); //setting it to 1 because the signal is currentindexCHANGED, i am so stupid lol
    moves->setCurrentIndex(0);
}

void AttackMoveWindow::createModifierGroupbox() {
    move_modifier_groupbox = new QGroupBox(tr("Modifiers:"));

    atk_modifier_groupbox = new QGroupBox(tr("Attacking Pokemon Modifiers:"));

    QHBoxLayout* attacking_layout = new QHBoxLayout;
    atk_modifier_groupbox->setLayout(attacking_layout);

    QLabel* atk_modifier_label = new QLabel;
    atk_modifier_label->setObjectName("attacking_atk_modifier_label");
    atk_modifier_label->setText(tr("Attack modifier"));
    attacking_layout->addWidget(atk_modifier_label);

    QSpinBox* atk_modifier_spinbox = new QSpinBox;
    atk_modifier_spinbox->setObjectName("attacking_atk_modifier");
    atk_modifier_spinbox->setRange(-6, 6);
    attacking_layout->addWidget(atk_modifier_spinbox, Qt::AlignLeft);

    QLabel* hits_modifier_label = new QLabel;
    hits_modifier_label->setText(tr("Number of hits:"));
    attacking_layout->addWidget(hits_modifier_label);

    QSpinBox* hits_modifier_spinbox = new QSpinBox;
    hits_modifier_spinbox->setObjectName("attacking_hits_modifier");
    hits_modifier_spinbox->setRange(1, 4);
    hits_modifier_spinbox->setSuffix("HKO");
    attacking_layout->addWidget(hits_modifier_spinbox, Qt::AlignLeft);

    move_modifier_groupbox = new QGroupBox("Modifiers:");

    QHBoxLayout* modifier_layout = new QHBoxLayout;
    modifier_layout->setAlignment(Qt::AlignLeft);

    move_modifier_groupbox->setLayout(modifier_layout);

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

void AttackMoveWindow::setMove(int index) {
    Move selected_move((Moves)index);

    move_groupbox->findChild<QComboBox*>("movetypes_combobox")->setCurrentIndex(selected_move.getMoveType());

    QComboBox* target = move_groupbox->findChild<QComboBox*>("target_combobox");
    if( selected_move.isSpread() ) { target->setCurrentIndex(Move::Target::DOUBLE); target->setVisible(true); }
    else { target->setCurrentIndex(Move::Target::SINGLE); target->setVisible(false); }

    move_groupbox->findChild<QComboBox*>("movecategories_combobox")->setCurrentIndex(selected_move.getMoveCategory());
    move_groupbox->findChild<QSpinBox*>("movebp_spinbox")->setValue(selected_move.getBasePower());

    if( selected_move.isSignatureZ() ) move_groupbox->findChild<QCheckBox*>("z")->setEnabled(false);
    else move_groupbox->findChild<QCheckBox*>("z")->setEnabled(true);

    if( selected_move.getMoveCategory() == Move::Category::PHYSICAL ) {
        defending_pokemon_groupbox->findChild<QLabel*>("def_iv_label")->setText(tr("Def IV"));
        defending_pokemon_groupbox->findChild<QLabel*>("def_ev_label")->setText(tr("Def EV"));
        defending_pokemon_groupbox->findChild<QLabel*>("def_modifier_label")->setText(tr("Def Modifier"));
        atk_modifier_groupbox->findChild<QLabel*>("attacking_atk_modifier_label")->setText(tr("Attack Modifier"));
    }

    else {
        defending_pokemon_groupbox->findChild<QLabel*>("def_iv_label")->setText(tr("Sp. Def IV"));
        defending_pokemon_groupbox->findChild<QLabel*>("def_ev_label")->setText(tr("Sp. Def EV"));
        defending_pokemon_groupbox->findChild<QLabel*>("def_modifier_label")->setText(tr("Sp. Def Modifier"));
        atk_modifier_groupbox->findChild<QLabel*>("attacking_atk_modifier_label")->setText(tr("Sp. Attack Modifier"));
    }
}
void AttackMoveWindow::setSpecies(int index) {
    Pokemon selected_pokemon(index + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = defending_pokemon_groupbox->findChild<QLabel*>("def_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting correct types
    QComboBox* type1 = defending_pokemon_groupbox->findChild<QComboBox*>("def_type1_combobox");
    type1->setCurrentIndex(selected_pokemon.getTypes()[0][0]);

    QComboBox* type2 = defending_pokemon_groupbox->findChild<QComboBox*>("def_type2_combobox");
    type2->setCurrentIndex(selected_pokemon.getTypes()[0][1]);

    if( selected_pokemon.getTypes()[0][0] == selected_pokemon.getTypes()[0][1] ) { type2->setVisible(false); type2->setVisible(false); }
    else { type2->setVisible(true); type2->setVisible(true); }

    //setting correct form
    QComboBox* form = defending_pokemon_groupbox->findChild<QComboBox*>("def_forms_combobox");
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
    QComboBox* ability = defending_pokemon_groupbox->findChild<QComboBox*>("def_abilities_combobox");
    ability->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0][0]);
}

void AttackMoveWindow::setForm(int index) {
    Pokemon selected_pokemon(defending_pokemon_groupbox->findChild<QComboBox*>("def_species_combobox")->currentIndex() + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path;
    bool load_result;

    if( index == 0 ) sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    else sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + "-" + QString::number(index) + ".png";

    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = defending_pokemon_groupbox->findChild<QLabel*>("def_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting correct types
    QComboBox* type1 = defending_pokemon_groupbox->findChild<QComboBox*>("def_type1_combobox");
    type1->setCurrentIndex(selected_pokemon.getTypes()[index][0]);

    QComboBox* type2 = defending_pokemon_groupbox->findChild<QComboBox*>("def_type2_combobox");
    type2->setCurrentIndex(selected_pokemon.getTypes()[index][1]);

    if( selected_pokemon.getTypes()[index][0] == selected_pokemon.getTypes()[index][1] ) { type2->setVisible(false); type2->setVisible(false); }
    else { type2->setVisible(true); type2->setVisible(true); }

    //setting correct ability
    QComboBox* ability = defending_pokemon_groupbox->findChild<QComboBox*>("def_abilities_combobox");
    ability->setCurrentIndex(selected_pokemon.getPossibleAbilities()[index][0]);
}

void AttackMoveWindow::solveMove(void) {
    Pokemon attacking1(defending_pokemon_groupbox->findChild<QComboBox*>("def_species_combobox")->currentIndex()+1);
    attacking1.setForm(defending_pokemon_groupbox->findChild<QComboBox*>("def_forms_combobox")->currentIndex());

    attacking1.setType(0, (Type)defending_pokemon_groupbox->findChild<QComboBox*>("def_type1_combobox")->currentIndex());
    attacking1.setType(1, (Type)defending_pokemon_groupbox->findChild<QComboBox*>("def_type2_combobox")->currentIndex());
    attacking1.setNature((Stats::Nature)defending_pokemon_groupbox->findChild<QComboBox*>("def_nature_combobox")->currentIndex());
    attacking1.setAbility((Ability)defending_pokemon_groupbox->findChild<QComboBox*>("def_abilities_combobox")->currentIndex());
    attacking1.setItem(Item(defending_pokemon_groupbox->findChild<QComboBox*>("def_items_combobox")->currentIndex()));

    Move attacking1_move((Moves)move_groupbox->findChild<QComboBox*>("moves_combobox")->currentIndex());
    if( move_groupbox->findChild<QComboBox*>("target_combobox")->currentIndex() == Move::Target::SINGLE ) attacking1_move.setTarget(Move::Target::SINGLE);
    else attacking1_move.setTarget(Move::Target::DOUBLE);
    attacking1_move.setMoveType((Type)move_groupbox->findChild<QComboBox*>("movetypes_combobox")->currentIndex());
    attacking1_move.setMoveCategory((Move::Category)move_groupbox->findChild<QComboBox*>("movecategories_combobox")->currentIndex());
    attacking1_move.setBasePower(move_groupbox->findChild<QSpinBox*>("movebp_spinbox")->value());

    if( move_groupbox->findChild<QCheckBox*>("crit")->checkState() == Qt::Checked ) attacking1_move.setCrit(true);
    else if(move_groupbox->findChild<QCheckBox*>("crit")->checkState() == Qt::Unchecked ) attacking1_move.setCrit(false);

    if( move_groupbox->findChild<QCheckBox*>("z")->checkState() == Qt::Checked ) attacking1_move.setZ(true);
    else if( move_groupbox->findChild<QCheckBox*>("z")->checkState() == Qt::Unchecked ) attacking1_move.setZ(false);

    attacking1_move.setWeather((Move::Weather)move_modifier_groupbox->findChild<QComboBox*>("weather_combobox")->currentIndex());
    attacking1_move.setTerrain((Move::Terrain)move_modifier_groupbox->findChild<QComboBox*>("terrain_combobox")->currentIndex());

    //now setting pokemon 1 iv/ev/modifier
    Stats::Stat stat;
    if( attacking1_move.getMoveCategory() == Move::Category::PHYSICAL ) stat = Stats::DEF;
    else stat = Stats::SPDEF;

    attacking1.setIV(stat, defending_pokemon_groupbox->findChild<QSpinBox*>("def_iv_spinbox")->value());
    attacking1.setEV(stat, defending_pokemon_groupbox->findChild<QSpinBox*>("def_ev_spinbox")->value());
    attacking1.setModifier(stat, defending_pokemon_groupbox->findChild<QSpinBox*>("def_modifier_spinbox")->value());

    attacking1.setIV(Stats::HP, defending_pokemon_groupbox->findChild<QSpinBox*>("hp_iv_spinbox")->value());
    attacking1.setEV(Stats::HP, defending_pokemon_groupbox->findChild<QSpinBox*>("hp_ev_spinbox")->value());
    attacking1.setCurrentHPPercentage(defending_pokemon_groupbox->findChild<QSpinBox*>("hp_perc_spinbox")->value());

    Turn turn;
    turn.addMove(Pokemon(1), attacking1_move); //adding a random pokemon in the turn since when using the turn class in an offensive manner Pokemon is ignored
    turn.setHits(atk_modifier_groupbox->findChild<QSpinBox*>("attacking_hits_modifier")->value());

    int atk_mod;
    int spatk_mod;
    if( attacking1_move.getMoveCategory() == Move::Category::PHYSICAL ) { spatk_mod = 0; /*useless*/ atk_mod = atk_modifier_groupbox->findChild<QSpinBox*>("attacking_atk_modifier")->value(); }
    else { atk_mod = 0; spatk_mod = atk_modifier_groupbox->findChild<QSpinBox*>("attacking_atk_modifier")->value(); }

    ((MainWindow*)parentWidget())->addAttackTurn(turn, attacking1, std::make_pair(atk_mod, spatk_mod));
}

void AttackMoveWindow::setAsBlank() {
    defending_pokemon_groupbox->findChild<QComboBox*>("def_species_combobox")->setCurrentIndex(0);
    defending_pokemon_groupbox->findChild<QComboBox*>("def_forms_combobox")->setCurrentIndex(0);
    defending_pokemon_groupbox->findChild<QComboBox*>("def_nature_combobox")->setCurrentIndex(0);
    defending_pokemon_groupbox->findChild<QComboBox*>("def_items_combobox")->setCurrentIndex(0);

    defending_pokemon_groupbox->findChild<QSpinBox*>("def_iv_spinbox")->setValue(31);
    defending_pokemon_groupbox->findChild<QSpinBox*>("def_ev_spinbox")->setValue(0);
    defending_pokemon_groupbox->findChild<QSpinBox*>("def_modifier_spinbox")->setValue(0);

    defending_pokemon_groupbox->findChild<QSpinBox*>("hp_iv_spinbox")->setValue(31);
    defending_pokemon_groupbox->findChild<QSpinBox*>("hp_ev_spinbox")->setValue(0);
    defending_pokemon_groupbox->findChild<QSpinBox*>("hp_perc_spinbox")->setValue(100);

    move_groupbox->findChild<QComboBox*>("moves_combobox")->setCurrentIndex(0);
    move_groupbox->findChild<QCheckBox*>("crit")->setChecked(false);
    move_groupbox->findChild<QCheckBox*>("z")->setChecked(false);

    move_modifier_groupbox->findChild<QComboBox*>("weather_combobox")->setCurrentIndex(0);
    move_modifier_groupbox->findChild<QComboBox*>("terrain_combobox")->setCurrentIndex(0);

    atk_modifier_groupbox->findChild<QSpinBox*>("attacking_atk_modifier")->setValue(0);
    atk_modifier_groupbox->findChild<QSpinBox*>("attacking_hits_modifier")->setValue(1);
}

void AttackMoveWindow::setAsTurn(const Turn& theTurn, const Pokemon& theDefendingPokemon, const attack_modifier& theAttackModifier) {
    defending_pokemon_groupbox->findChild<QComboBox*>("def_species_combobox")->setCurrentIndex(theDefendingPokemon.getPokedexNumber()-1);
    defending_pokemon_groupbox->findChild<QComboBox*>("def_type1_combobox")->setCurrentIndex(theDefendingPokemon.getTypes()[theTurn.getMoves()[0].first.getForm()][0]);
    defending_pokemon_groupbox->findChild<QComboBox*>("def_type2_combobox")->setCurrentIndex(theDefendingPokemon.getTypes()[theTurn.getMoves()[0].first.getForm()][1]);
    defending_pokemon_groupbox->findChild<QComboBox*>("def_forms_combobox")->setCurrentIndex(theDefendingPokemon.getForm());
    defending_pokemon_groupbox->findChild<QComboBox*>("def_nature_combobox")->setCurrentIndex(theDefendingPokemon.getNature());
    defending_pokemon_groupbox->findChild<QComboBox*>("def_items_combobox")->setCurrentIndex(theDefendingPokemon.getItem().getIndex());
    defending_pokemon_groupbox->findChild<QComboBox*>("def_abilities_combobox")->setCurrentIndex(theDefendingPokemon.getAbility());

    Stats::Stat stat;
    if( theTurn.getMoves()[0].second.getMoveCategory() == Move::Category::SPECIAL ) stat = Stats::SPDEF;
    else stat = Stats::DEF;

    defending_pokemon_groupbox->findChild<QSpinBox*>("def_iv_spinbox")->setValue(theDefendingPokemon.getIV(stat));
    defending_pokemon_groupbox->findChild<QSpinBox*>("def_ev_spinbox")->setValue(theDefendingPokemon.getEV(stat));
    defending_pokemon_groupbox->findChild<QSpinBox*>("def_modifier_spinbox")->setValue(theDefendingPokemon.getModifier(stat));
    defending_pokemon_groupbox->findChild<QSpinBox*>("hp_iv_spinbox")->setValue(theDefendingPokemon.getIV(Stats::HP));
    defending_pokemon_groupbox->findChild<QSpinBox*>("hp_ev_spinbox")->setValue(theDefendingPokemon.getEV(Stats::HP));
    defending_pokemon_groupbox->findChild<QSpinBox*>("hp_perc_spinbox")->setValue(theDefendingPokemon.getCurrentHPPercentage());

    move_groupbox->findChild<QComboBox*>("moves_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getMoveIndex());
    move_groupbox->findChild<QComboBox*>("target_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getTarget());
    move_groupbox->findChild<QComboBox*>("movetypes_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getMoveType());
    move_groupbox->findChild<QComboBox*>("movecategories_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getMoveCategory());
    move_groupbox->findChild<QSpinBox*>("movebp_spinbox")->setValue(theTurn.getMoves()[0].second.getBasePower());

    move_groupbox->findChild<QCheckBox*>("crit")->setChecked(theTurn.getMoves()[0].second.isCrit());
    move_groupbox->findChild<QCheckBox*>("z")->setChecked(theTurn.getMoves()[0].second.isZ());

    move_modifier_groupbox->findChild<QComboBox*>("weather_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getWeather());
    move_modifier_groupbox->findChild<QComboBox*>("terrain_combobox")->setCurrentIndex(theTurn.getMoves()[0].second.getTerrain());


    if( theTurn.getMoves()[0].second.getMoveCategory() == Move::Category::SPECIAL ) atk_modifier_groupbox->findChild<QSpinBox*>("attacking_atk_modifier")->setValue(std::get<1>(theAttackModifier));
    else atk_modifier_groupbox->findChild<QSpinBox*>("attacking_atk_modifier")->setValue(std::get<0>(theAttackModifier));

    atk_modifier_groupbox->findChild<QSpinBox*>("attacking_hits_modifier")->setValue(theTurn.getHits());
}
