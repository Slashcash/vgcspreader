#include "mainwindow.hpp"

#include <QFile>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextStream>
#include <QSpacerItem>
#include <QHeaderView>
#include <QFormLayout>
#include <QDebug>

#include "pokemon.hpp"

MainWindow::MainWindow() {
    setObjectName("MainWindow");
    setWindowTitle("VGCSpreader");

    createDefendingPokemonGroupBox();
    createMovesGroupBox();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(defending_groupbox);
    main_layout->addWidget(moves_groupbox);

    setLayout(main_layout);

    QFile moves_input(":/db/moves.txt");
    moves_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_moves(&moves_input);
    while (!in_moves.atEnd()) {
        QString line = in_moves.readLine();
        moves_names.push_back(line);
    }

    move_window = new MoveWindow(this);
    move_window->setObjectName("MoveWindow");
    move_window->setWindowTitle("VGCSpreader");

    result_window = new ResultWindow(this);
    result_window->setObjectName("ResultWindow");
    result_window->setWindowTitle("VGCSpreader");

    //bottom buttons
    bottom_buttons = new QDialogButtonBox;

    bottom_buttons->addButton(tr("Clear"), QDialogButtonBox::ButtonRole::RejectRole);
    bottom_buttons->addButton(tr("Calculate"), QDialogButtonBox::ButtonRole::AcceptRole);

    main_layout->addWidget(bottom_buttons, Qt::AlignRight);

    //SIGNAL
    connect(move_window->bottom_button_box, SIGNAL(accepted()), this, SLOT(solveMove()));
    connect(bottom_buttons, SIGNAL(rejected()), this, SLOT(clear()));
    connect(bottom_buttons, SIGNAL(accepted()), this, SLOT(calculate()));

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void MainWindow::setButtonClickable(int row, int column) {
    moves_edit_button->setEnabled(true);
    moves_delete_button->setEnabled(true);
}

void MainWindow::setDefendingPokemonSpecies(int index) {
    Pokemon selected_pokemon(index + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = defending_groupbox->findChild<QLabel*>("defending_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting ability
    defending_groupbox->findChild<QComboBox*>("defending_abilities_combobox")->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0][0]);

    //setting correct types
    defending_groupbox->findChild<QComboBox*>("defending_type1_combobox")->setCurrentIndex(selected_pokemon.getTypes()[0][0]);
    defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->setCurrentIndex(selected_pokemon.getTypes()[0][1]);

    if( selected_pokemon.getTypes()[0][0] == selected_pokemon.getTypes()[0][1] ) defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->setVisible(false);
    else defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->setVisible(true);

    //setting correct form
    if( selected_pokemon.getFormesNumber() > 1 ) {
        defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->clear();
        for(unsigned int i = 0; i < selected_pokemon.getFormesNumber(); i++) defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->addItem(retrieveFormName(index+1, i));
        defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->setCurrentIndex(0);
        defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->setVisible(true);
    }

    else {
        defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->setVisible(false);
        defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->setCurrentIndex(0);
    }
}

void MainWindow::setDefendingPokemonForm(int index) {
    Pokemon selected_pokemon(defending_groupbox->findChild<QComboBox*>("defending_species_combobox")->currentIndex() + 1);

    //setting correct sprite
    QPixmap sprite_pixmap;
    QString sprite_path;
    bool load_result;

    if( index == 0 ) sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + ".png";
    else sprite_path = ":/db/sprites/" + QString::number(selected_pokemon.getPokedexNumber()) + "-" + QString::number(index) + ".png";

    sprite_pixmap.load(sprite_path);
    const int SPRITE_SCALE_FACTOR = 2;
    sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

    QLabel* sprite = defending_groupbox->findChild<QLabel*>("defending_sprite");
    sprite->setPixmap(sprite_pixmap);

    //setting ability
    defending_groupbox->findChild<QComboBox*>("defending_abilities_combobox")->setCurrentIndex(selected_pokemon.getPossibleAbilities()[index][0]);

    //setting correct types
    defending_groupbox->findChild<QComboBox*>("defending_type1_combobox")->setCurrentIndex(selected_pokemon.getTypes()[index][0]);
    defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->setCurrentIndex(selected_pokemon.getTypes()[index][1]);

    if( selected_pokemon.getTypes()[index][0] == selected_pokemon.getTypes()[index][1] ) defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->setVisible(false);
    else defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->setVisible(true);
}

void MainWindow::eraseMove(bool checked) {
    turns.erase(turns.begin() + moves_view->currentRow());
    modifiers.erase(modifiers.begin() + moves_view->currentRow());

    moves_view->removeRow(moves_view->currentRow());

    if( turns.empty() ) {
        moves_edit_button->setEnabled(false);
        moves_delete_button->setEnabled(false);
    }
}

void MainWindow::createDefendingPokemonGroupBox() {
    defending_groupbox = new QGroupBox(tr("Defending Pokemon:"));

    //the main horizontal layout for this window
    QHBoxLayout* defending_layout = new QHBoxLayout;

    //the groupbox in which we encapsulate this window
    defending_groupbox->setLayout(defending_layout);

    //---SPECIES & TYPES---//

    //---species---//
    //the main layout for this entire section
    QVBoxLayout* species_types_layout = new QVBoxLayout;
    species_types_layout->setSpacing(1);
    species_types_layout->setAlignment(Qt::AlignVCenter);

    //---sprite---
    QHBoxLayout* sprite_layout = new QHBoxLayout;

    QLabel* sprite = new QLabel;
    sprite->setObjectName("defending_sprite");
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
    species->setObjectName("defending_species_combobox");

    //loading species names
    QFile species_input(":/db/species.txt");
    species_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_species(&species_input);
    bool is_egg = true; //WE DO NOT WANT TO ADD THE EGG
    while (!in_species.atEnd()) {
        QString line = in_species.readLine();
        if( !is_egg ) species_names.push_back(line);
        else is_egg = false;
    }

    for(auto it = species_names.begin(); it < species_names.end(); it++) species->addItem(*it);

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
    forms->setObjectName("defending_forms_combobox");

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
    types1->setObjectName("defending_type1_combobox");
    types2->setObjectName("defending_type2_combobox");

    //loading type names
    QFile types_input(":/db/types.txt");
    types_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_types(&types_input);
    while (!in_types.atEnd()) {
        QString line = in_types.readLine();
        types_names.push_back(line);
    }

    for(auto it = types_names.begin(); it < types_names.end(); it++) { types1->addItem(*it); types2->addItem(*it); }

    //resizing them
    int types_width = species->minimumSizeHint().width();
    types1->setMaximumWidth(types_width);
    types2->setMaximumWidth(types_width);

    //adding them to the layout
    types_layout->addWidget(types1);
    types_layout->addWidget(types2);
    species_types_layout->addLayout(types_layout);

    //adding everything to the window
    defending_layout->addLayout(species_types_layout);

    //and then some spacing
    defending_layout->insertSpacing(1, 35);

    //---MAIN FORM---//
    QVBoxLayout* main_form_layout = new QVBoxLayout;

    //---information section---///
    QFormLayout* form_layout = new QFormLayout;
    main_form_layout->addLayout(form_layout);

    //NATURE
    //the combobox for the nature
    QComboBox* natures = new QComboBox;
    natures->setObjectName("defending_nature_combobox");

    //loading nature names
    QFile natures_input(":/db/natures.txt");
    natures_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_natures(&natures_input);
    while (!in_natures.atEnd()) {
        QString line = in_natures.readLine();
        natures_names.push_back(line);
    }

    for(auto it = natures_names.begin(); it < natures_names.end(); it++) natures->addItem(*it);

    form_layout->addRow(tr("Nature:"), natures);

    //ABILITY
    //the combobox for the ability
    QComboBox* abilities = new QComboBox;
    abilities->setObjectName("defending_abilities_combobox");

    //loading abilities names
    QFile abilities_input(":/db/abilities.txt");
    abilities_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_abilities(&abilities_input);
    while (!in_abilities.atEnd()) {
        QString line = in_abilities.readLine();
        abilities_names.push_back(line);
    }

    for(auto it = abilities_names.begin(); it < abilities_names.end(); it++) abilities->addItem(*it);

    //getting the abilities width (to be used later)
    int abilities_width = abilities->minimumSizeHint().width();

    form_layout->addRow(tr("Ability:"), abilities);

    //ITEM
    //the combobox for the item
    QComboBox* items = new QComboBox;
    items->setObjectName("defending_items_combobox");

    //loading item names
    QFile items_input(":/db/items.txt");
    items_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_items(&items_input);
    while (!in_items.atEnd()) {
        QString line = in_items.readLine();
        items_names.push_back(line);
    }

    for(auto it = items_names.begin(); it < items_names.end(); it++) items->addItem(*it);

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

    //hp iv
    QLabel* hpiv_label = new QLabel(tr("HP IV:"));
    hpiv_label->setObjectName("defending_hpiv_label");
    modifiers_layout->addWidget(hpiv_label);

    QSpinBox* hpiv = new QSpinBox;
    hpiv->setObjectName("defending_hpiv_spinbox");
    hpiv->setRange(0, 31);
    hpiv->setValue(31);
    modifiers_layout->addWidget(hpiv);

    //def iv
    QLabel* defiv_label = new QLabel(tr("Def IV:"));
    defiv_label->setObjectName("defending_defiv_label");
    modifiers_layout->addWidget(defiv_label);

    QSpinBox* defiv = new QSpinBox;
    defiv->setRange(0, 31);
    defiv->setValue(31);
    defiv->setObjectName("defending_defiv_spinbox");
    modifiers_layout->addWidget(defiv);

    //spdef iv
    QLabel* defending_spdefiv_label = new QLabel(tr("Sp.Def IV:"));
    defending_spdefiv_label->setObjectName("defending_spdefiv_label");
    modifiers_layout->addWidget(defending_spdefiv_label);

    QSpinBox* spdefiv = new QSpinBox;
    spdefiv->setObjectName("defending_spdefiv_spinbox");
    spdefiv->setRange(0, 31);
    spdefiv->setValue(31);
    modifiers_layout->addWidget(spdefiv);

    main_form_layout->addLayout(modifiers_layout);

    //---assigned evs---//
    QHBoxLayout* assigned_layout = new QHBoxLayout;
    assigned_layout->setAlignment(Qt::AlignLeft);
    assigned_layout->setSpacing(10);

    //atk evs
    QLabel* atkev_label = new QLabel(tr("Atk EV:"));
    assigned_layout->addWidget(atkev_label);

    QSpinBox* atkev = new QSpinBox;
    atkev->setObjectName("defending_atkev_spinbox");
    atkev->setRange(0, 252);
    assigned_layout->addWidget(atkev);

    //spatk evs
    QLabel* spatkev_label = new QLabel(tr("Sp. Atk EV:"));
    assigned_layout->addWidget(spatkev_label);

    QSpinBox* spatkev = new QSpinBox;
    spatkev->setRange(0, 252);
    spatkev->setObjectName("defending_spatkev_spinbox");
    assigned_layout->addWidget(spatkev);

    //spe ev
    QLabel* speev_label = new QLabel(tr("Speed EV:"));
    assigned_layout->addWidget(speev_label);

    QSpinBox* speev = new QSpinBox;
    speev->setObjectName("defending_speev_spinbox");
    speev->setRange(0, 252);
    assigned_layout->addWidget(speev);

    main_form_layout->addLayout(assigned_layout);

    //adding everything to the layout
    defending_layout->addLayout(main_form_layout);

    //CONNECTING SIGNALS
    connect(species, SIGNAL(currentIndexChanged(int)), this, SLOT(setDefendingPokemonSpecies(int)));
    connect(forms, SIGNAL(currentIndexChanged(int)), this, SLOT(setDefendingPokemonForm(int)));

    //setting index 0
    species->setCurrentIndex(1); //setting it to 1 because the signal is currentindexCHANGED, i am so stupid lol
    species->setCurrentIndex(0);

}

void MainWindow::createMovesGroupBox() {
    moves_groupbox = new QGroupBox(tr("Moves:"));

    moves_layout = new QGridLayout;

    moves_groupbox->setLayout(moves_layout);

    //MOVES ADD BUTTON
    moves_add_button = new QPushButton(tr("Add"));
    moves_layout->addWidget(moves_add_button, 0, 0);

    //MOVES EDIT BUTTON
    moves_edit_button = new QPushButton(tr("Edit"));
    moves_edit_button->setEnabled(false);
    moves_layout->addWidget(moves_edit_button, 1, 0);

    //MOVES DELETE BUTTON
    moves_delete_button = new QPushButton(tr("Delete"));
    moves_delete_button->setEnabled(false);
    moves_layout->addWidget(moves_delete_button, 2, 0);

    //MOVES VIEW
    moves_view = new QTableWidget(0, 3);
    moves_layout->addWidget(moves_view, 0, 1, 10, 10);
    moves_view->horizontalHeader()->setVisible(false);
    moves_view->verticalHeader()->setVisible(false);
    moves_view->setShowGrid(false);
    moves_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    moves_view->setColumnWidth(1, 20);

    //CONNECTING SIGNALS
    connect(moves_view, SIGNAL(cellClicked(int,int)), this, SLOT(setButtonClickable(int,int)));
    connect(moves_delete_button, SIGNAL(clicked(bool)), this, SLOT(eraseMove(bool)));
    connect(moves_add_button, SIGNAL(clicked(bool)), this, SLOT(openMoveWindow(bool)));
    connect(moves_edit_button, SIGNAL(clicked(bool)), this, SLOT(openMoveWindowEdit(bool)));
}

void MainWindow::solveMove() {
    if( !move_window->isEditMode() ) {
        moves_view->setRowCount(moves_view->rowCount()+1);

        auto buffer = turns.back().getMoves();

        QString move_name_1;
        if( buffer[0].second.isZ() ) move_name_1 = tr("Z-") + moves_names[buffer[0].second.getMoveIndex()];
        else move_name_1 = moves_names[buffer[0].second.getMoveIndex()];
        QString move1(species_names[buffer[0].first.getPokedexNumber()-1] + " " + move_name_1);

        moves_view->setItem(turns.size()-1, 0, new QTableWidgetItem(move1));

        moves_view->resizeColumnToContents(0);

        if( turns.back().getMoveNum() > 1 ) {
            QString move_name_2;
            if( buffer[1].second.isZ() ) move_name_2 = tr("Z-") + moves_names[buffer[1].second.getMoveIndex()];
            else move_name_2 = moves_names[buffer[1].second.getMoveIndex()];

            QString move2(species_names[buffer[1].first.getPokedexNumber()-1] + " " + move_name_2);

            QTableWidgetItem* plus_sign = new QTableWidgetItem("+");
            plus_sign->setTextAlignment(Qt::AlignCenter);
            moves_view->setItem(turns.size()-1, 1, plus_sign);
            moves_view->setItem(turns.size()-1, 2, new QTableWidgetItem(move2));
            moves_view->resizeColumnToContents(2);
        }
    }

    else {
        auto buffer = turns[moves_view->currentRow()].getMoves();

        QString move_name_1;
        if( buffer[0].second.isZ() ) move_name_1 = tr("Z-") + moves_names[buffer[0].second.getMoveIndex()];
        else move_name_1 = moves_names[buffer[0].second.getMoveIndex()];
        QString move1(species_names[buffer[0].first.getPokedexNumber()-1] + " " + move_name_1);

        moves_view->setItem(moves_view->currentRow(), 0, new QTableWidgetItem(move1));

        moves_view->resizeColumnToContents(0);

        if( turns[moves_view->currentRow()].getMoveNum() > 1 ) {
            QString move_name_2;
            if( buffer[1].second.isZ() ) move_name_2 = tr("Z-") + moves_names[buffer[1].second.getMoveIndex()];
            else move_name_2 = moves_names[buffer[1].second.getMoveIndex()];

            QString move2(species_names[buffer[1].first.getPokedexNumber()-1] + " " + move_name_2);

            QTableWidgetItem* plus_sign = new QTableWidgetItem("+");
            plus_sign->setTextAlignment(Qt::AlignCenter);
            moves_view->setItem(moves_view->currentRow(), 1, plus_sign);
            moves_view->setItem(moves_view->currentRow(), 2, new QTableWidgetItem(move2));
            moves_view->resizeColumnToContents(2);
        }

        else {
            moves_view->takeItem(moves_view->currentRow(), 1);
            moves_view->takeItem(moves_view->currentRow(), 2);
        }
    }
}

void MainWindow::openMoveWindow(bool checked) {
    move_window->setAsBlank();
    move_window->setEditMode(false);
    move_window->setModal(true);
    move_window->setVisible(true);
}

void MainWindow::openMoveWindowEdit(bool checked) {
    move_window->setAsBlank();
    move_window->setAsTurn(turns[moves_view->currentRow()], modifiers[moves_view->currentRow()]);
    move_window->setEditMode(true);
    move_window->setModal(true);
    move_window->setVisible(true);
}

void MainWindow::addTurn(const Turn& theTurn, const defense_modifier& theModifier) {
    if( !move_window->isEditMode() ) {
        turns.push_back(theTurn);
        modifiers.push_back(theModifier);
    }

    else {
        turns[moves_view->currentRow()] = theTurn;
        modifiers[moves_view->currentRow()] = theModifier;
    }
}

void MainWindow::clear() {
    defending_groupbox->findChild<QComboBox*>("defending_species_combobox")->setCurrentIndex(0);
    defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->setCurrentIndex(0);
    defending_groupbox->findChild<QComboBox*>("defending_nature_combobox")->setCurrentIndex(0);
    defending_groupbox->findChild<QComboBox*>("defending_items_combobox")->setCurrentIndex(0);
    defending_groupbox->findChild<QSpinBox*>("defending_hpiv_spinbox")->setValue(31);
    defending_groupbox->findChild<QSpinBox*>("defending_defiv_spinbox")->setValue(31);
    defending_groupbox->findChild<QSpinBox*>("defending_spdefiv_spinbox")->setValue(31);
    defending_groupbox->findChild<QSpinBox*>("defending_atkev_spinbox")->setValue(0);
    defending_groupbox->findChild<QSpinBox*>("defending_spatkev_spinbox")->setValue(0);
    defending_groupbox->findChild<QSpinBox*>("defending_speev_spinbox")->setValue(0);

    moves_view->clear();
    turns.clear();
    modifiers.clear();
}

void MainWindow::calculate() {
    Pokemon selected_pokemon(defending_groupbox->findChild<QComboBox*>("defending_species_combobox")->currentIndex()+1);
    selected_pokemon.setForm(defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->currentIndex());
    selected_pokemon.setType(0, (Type)defending_groupbox->findChild<QComboBox*>("defending_type1_combobox")->currentIndex());
    selected_pokemon.setType(1, (Type)defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->currentIndex());
    selected_pokemon.setNature((Stats::Nature)defending_groupbox->findChild<QComboBox*>("defending_nature_combobox")->currentIndex());
    selected_pokemon.setAbility((Ability)defending_groupbox->findChild<QComboBox*>("defending_abilities_combobox")->currentIndex());
    selected_pokemon.setItem(Item(defending_groupbox->findChild<QComboBox*>("defending_items_combobox")->currentIndex()));
    selected_pokemon.setIV(Stats::HP, defending_groupbox->findChild<QSpinBox*>("defending_hpiv_spinbox")->value());
    selected_pokemon.setIV(Stats::DEF, defending_groupbox->findChild<QSpinBox*>("defending_defiv_spinbox")->value());
    selected_pokemon.setIV(Stats::SPDEF, defending_groupbox->findChild<QSpinBox*>("defending_spdefiv_spinbox")->value());
    selected_pokemon.setEV(Stats::ATK, defending_groupbox->findChild<QSpinBox*>("defending_atkev_spinbox")->value());
    selected_pokemon.setEV(Stats::SPATK, defending_groupbox->findChild<QSpinBox*>("defending_spatkev_spinbox")->value());
    selected_pokemon.setEV(Stats::SPE, defending_groupbox->findChild<QSpinBox*>("defending_speev_spinbox")->value());

    std::vector<float> rolls;
    auto result = selected_pokemon.resistMove(turns, modifiers, rolls);

    //THIS SHOULD PROBABLY BE CALCULATED INSIDE THE resistMove FUNCTION; WE'LL SEE
    selected_pokemon.setEV(Stats::HP, std::get<0>(result));
    selected_pokemon.setEV(Stats::DEF, std::get<1>(result));
    selected_pokemon.setEV(Stats::SPDEF, std::get<2>(result));

    std::vector<std::vector<float>> damages;
    std::vector<std::vector<int>> int_damages;

    for( auto it = 0; it < turns.size(); it++ ) {
        selected_pokemon.setModifier(Stats::HP, std::get<0>(modifiers[it]));
        selected_pokemon.setModifier(Stats::DEF, std::get<1>(modifiers[it]));
        selected_pokemon.setModifier(Stats::SPDEF, std::get<2>(modifiers[it]));
        damages.push_back(selected_pokemon.getDamagePercentage(turns[it]));
        int_damages.push_back(selected_pokemon.getDamageInt(turns[it]));
    }

    for(auto it = int_damages.begin(); it < int_damages.end(); it++) qDebug() << *it;

    result_window->setModal(true);
    result_window->setResult(selected_pokemon, modifiers, turns, result, damages, int_damages, rolls);
    result_window->show();
}

QString MainWindow::retrieveFormName(const int species, const int form) {
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
