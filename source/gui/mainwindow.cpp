#include "mainwindow.hpp"

#include <QFile>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextStream>
#include <QSpacerItem>
#include <QHeaderView>
#include <QDebug>

#include "pokemon.hpp"

MainWindow::MainWindow() {
    setObjectName("MainWindow");
    setWindowTitle("VGCSpreader");

    createDefendingPokemonGroupBox();
    createMovesGroupBox();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(defending_pokemon_groupbox);
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

    defending_pokemon_ability_combobox->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0]);

    //setting correct types
    defending_pokemon_type1_combobox->setCurrentIndex(selected_pokemon.getTypes()[0]);
    defending_pokemon_type2_combobox->setCurrentIndex(selected_pokemon.getTypes()[1]);

    if( selected_pokemon.getTypes()[0] == selected_pokemon.getTypes()[1] ) { defending_pokemon_type2_combobox->setVisible(false); defending_pokemon_type2_label->setVisible(false); }
    else { defending_pokemon_type2_combobox->setVisible(true); defending_pokemon_type2_label->setVisible(true); }
}

void MainWindow::eraseMove(bool checked) {
    turns.erase(turns.begin() + moves_view->currentRow());

    moves_view->removeRow(moves_view->currentRow());

    if( turns.empty() ) {
        moves_edit_button->setEnabled(false);
        moves_delete_button->setEnabled(false);
    }
}

void MainWindow::createDefendingPokemonGroupBox() {
    defending_pokemon_groupbox = new QGroupBox(tr("Defending Pokemon:"));

    defending_pokemon_layout = new QGridLayout;

    //DEFENDING POKEMON SPECIES
    defending_pokemon_species_label = new QLabel;
    defending_pokemon_species_label->setText(tr("Species:"));
    defending_pokemon_layout->addWidget(defending_pokemon_species_label, 0, 0, Qt::AlignRight);

    defending_pokemon_species_combobox = new QComboBox;

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

    for(auto it = species_names.begin(); it < species_names.end(); it++) defending_pokemon_species_combobox->addItem(*it);

    defending_pokemon_layout->addWidget(defending_pokemon_species_combobox, 0, 1);

    Pokemon selected_pokemon(defending_pokemon_species_combobox->currentIndex() + 1);

    //DEFENDING POKEMON TYPES
    defending_pokemon_type1_label = new QLabel;
    defending_pokemon_type1_label->setText(tr("Type 1:"));
    defending_pokemon_layout->addWidget(defending_pokemon_type1_label, 1, 0, Qt::AlignRight);
    defending_pokemon_type2_label = new QLabel;
    defending_pokemon_type2_label->setText(tr("Type 2:"));
    defending_pokemon_layout->addWidget(defending_pokemon_type2_label, 2, 0, Qt::AlignRight);

    defending_pokemon_type1_combobox = new QComboBox;
    defending_pokemon_type2_combobox = new QComboBox;

    //loading type names
    QFile types_input(":/db/types.txt");
    types_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_types(&types_input);
    while (!in_types.atEnd()) {
        QString line = in_types.readLine();
        types_names.push_back(line);
    }

    for(auto it = types_names.begin(); it < types_names.end(); it++) { defending_pokemon_type1_combobox->addItem(*it); defending_pokemon_type2_combobox->addItem(*it); }

    defending_pokemon_layout->addWidget(defending_pokemon_type1_combobox, 1, 1);
    defending_pokemon_layout->addWidget(defending_pokemon_type2_combobox, 2, 1);
    if( selected_pokemon.getTypes()[0] == selected_pokemon.getTypes()[1] ) defending_pokemon_type2_combobox->hide();

    //setting correct types
    defending_pokemon_type1_combobox->setCurrentIndex(selected_pokemon.getTypes()[0]);
    defending_pokemon_type2_combobox->setCurrentIndex(selected_pokemon.getTypes()[1]);

    //DRFRNDING POKEMON NATURE
    defending_pokemon_nature_label = new QLabel;
    defending_pokemon_nature_label->setText(tr("Nature:"));
    defending_pokemon_layout->addWidget(defending_pokemon_nature_label, 3, 0, Qt::AlignRight);

    defending_pokemon_nature_combobox = new QComboBox;

    //loading nature names
    QFile natures_input(":/db/natures.txt");
    natures_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_natures(&natures_input);
    while (!in_natures.atEnd()) {
        QString line = in_natures.readLine();
        natures_names.push_back(line);
    }

    for(auto it = natures_names.begin(); it < natures_names.end(); it++) defending_pokemon_nature_combobox->addItem(*it);

    defending_pokemon_layout->addWidget(defending_pokemon_nature_combobox, 3, 1);

    //DEFRNDING POKEMON ABILITY
    defending_pokemon_ability_label = new QLabel;
    defending_pokemon_ability_label->setText(tr("Ability:"));
    defending_pokemon_layout->addWidget(defending_pokemon_ability_label, 4, 0, Qt::AlignRight);

    defending_pokemon_ability_combobox = new QComboBox;

    //loading abilities names
    QFile abilities_input(":/db/abilities.txt");
    abilities_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_abilities(&abilities_input);
    while (!in_abilities.atEnd()) {
        QString line = in_abilities.readLine();
        abilities_names.push_back(line);
    }

    for(auto it = abilities_names.begin(); it < abilities_names.end(); it++) defending_pokemon_ability_combobox->addItem(*it);

    //setting the correct ability
    defending_pokemon_ability_combobox->setCurrentIndex(selected_pokemon.getPossibleAbilities()[0]);

    defending_pokemon_layout->addWidget(defending_pokemon_ability_combobox, 4, 1);

    //DEFENDING POKEMON ITEM
    defending_pokemon_item_label = new QLabel;
    defending_pokemon_item_label->setText(tr("Item:"));
    defending_pokemon_layout->addWidget(defending_pokemon_item_label, 5, 0, Qt::AlignRight);

    defending_pokemon_item_combobox = new QComboBox;

    //loading item names
    QFile items_input(":/db/items.txt");
    items_input.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in_items(&items_input);
    while (!in_items.atEnd()) {
        QString line = in_items.readLine();
        items_names.push_back(line);
    }

    for(auto it = items_names.begin(); it < items_names.end(); it++) defending_pokemon_item_combobox->addItem(*it);

    defending_pokemon_layout->addWidget(defending_pokemon_item_combobox, 5, 1);

    //spacer
    QSpacerItem* spacer = new QSpacerItem(8, 0);

    //DEFENDING POKEMON HP IV
    defending_pokemon_hpiv_label = new QLabel;
    defending_pokemon_hpiv_label->setText(tr("HP IV:"));
    defending_pokemon_layout->addWidget(defending_pokemon_hpiv_label, 6, 0, Qt::AlignRight);

    defending_pokemon_hpiv_spinbox = new QSpinBox;
    defending_pokemon_hpiv_spinbox->setRange(0, 31);
    defending_pokemon_layout->addWidget(defending_pokemon_hpiv_spinbox, 6, 1);
    defending_pokemon_hpiv_spinbox->setValue(31);

    //DEFENDING POKEMON DEF IV
    defending_pokemon_defiv_label = new QLabel;
    defending_pokemon_defiv_label->setText(tr("Defense IV:"));
    defending_pokemon_layout->addWidget(defending_pokemon_defiv_label, 7, 0, Qt::AlignRight);

    defending_pokemon_defiv_spinbox = new QSpinBox;
    defending_pokemon_defiv_spinbox->setRange(0, 31);
    defending_pokemon_layout->addWidget(defending_pokemon_defiv_spinbox, 7, 1);
    defending_pokemon_defiv_spinbox->setValue(31);

    //DEFENDING POKEMON SPDEF IV
    defending_pokemon_spdefiv_label = new QLabel;
    defending_pokemon_spdefiv_label->setText(tr("Sp. Defense IV:"));
    defending_pokemon_layout->addWidget(defending_pokemon_spdefiv_label, 8, 0, Qt::AlignRight);

    defending_pokemon_spdefiv_spinbox = new QSpinBox;
    defending_pokemon_spdefiv_spinbox->setRange(0, 31);
    defending_pokemon_layout->addWidget(defending_pokemon_spdefiv_spinbox, 8, 1);
    defending_pokemon_spdefiv_spinbox->setValue(31);

    //DEFENDING POKEMON ATTACK
    defending_pokemon_layout->addItem(spacer, 0 , 2);

    defending_pokemon_attack_label = new QLabel;
    defending_pokemon_attack_label->setText(tr("Attack EV:"));
    defending_pokemon_layout->addWidget(defending_pokemon_attack_label, 0, 3, Qt::AlignRight);

    defending_pokemon_attack_spinbox = new QSpinBox;
    defending_pokemon_attack_spinbox->setRange(0, 252);
    defending_pokemon_layout->addWidget(defending_pokemon_attack_spinbox, 0, 4);

    //DEFENDING POKEMON SPATTACK
    defending_pokemon_layout->addItem(spacer, 1 , 2);

    defending_pokemon_spattack_label = new QLabel;
    defending_pokemon_spattack_label->setText(tr("Sp. Attack EV:"));
    defending_pokemon_layout->addWidget(defending_pokemon_spattack_label, 1, 3, Qt::AlignRight);

    defending_pokemon_spattack_spinbox = new QSpinBox;
    defending_pokemon_spattack_spinbox->setRange(0, 252);
    defending_pokemon_layout->addWidget(defending_pokemon_spattack_spinbox, 1, 4);

    //DEFENDING POKEMON SPEED
    defending_pokemon_layout->addItem(spacer, 2 , 2);

    defending_pokemon_speed_label = new QLabel;
    defending_pokemon_speed_label->setText(tr("Speed EV:"));
    defending_pokemon_layout->addWidget(defending_pokemon_speed_label, 2, 3, Qt::AlignRight);

    defending_pokemon_speed_spinbox = new QSpinBox;
    defending_pokemon_speed_spinbox->setRange(0, 252);
    defending_pokemon_layout->addWidget(defending_pokemon_speed_spinbox, 2, 4);

    //SETTING THE LAYOUT
    defending_pokemon_groupbox->setLayout(defending_pokemon_layout);

    //CONNECTING SIGNALS
    connect(defending_pokemon_species_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDefendingPokemonSpecies(int)));
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
    defending_pokemon_species_combobox->setCurrentIndex(0);
    defending_pokemon_nature_combobox->setCurrentIndex(0);
    defending_pokemon_item_combobox->setCurrentIndex(0);
    defending_pokemon_hpiv_spinbox->setValue(31);
    defending_pokemon_defiv_spinbox->setValue(31);
    defending_pokemon_spdefiv_spinbox->setValue(31);
    defending_pokemon_attack_spinbox->setValue(0);
    defending_pokemon_spattack_spinbox->setValue(0);
    defending_pokemon_speed_spinbox->setValue(0);

    moves_view->clear();
    turns.clear();
    modifiers.clear();
}

void MainWindow::calculate() {
    Pokemon selected_pokemon(defending_pokemon_species_combobox->currentIndex()+1);
    selected_pokemon.setType(0, (Type)defending_pokemon_type1_combobox->currentIndex());
    selected_pokemon.setType(1, (Type)defending_pokemon_type2_combobox->currentIndex());
    selected_pokemon.setNature((Stats::Nature)defending_pokemon_nature_combobox->currentIndex());
    selected_pokemon.setAbility((Ability)defending_pokemon_ability_combobox->currentIndex());
    selected_pokemon.setItem(Item(defending_pokemon_item_combobox->currentIndex()));
    selected_pokemon.setIV(Stats::HP, defending_pokemon_hpiv_spinbox->value());
    selected_pokemon.setIV(Stats::DEF, defending_pokemon_defiv_spinbox->value());
    selected_pokemon.setIV(Stats::SPDEF, defending_pokemon_spdefiv_spinbox->value());
    selected_pokemon.setEV(Stats::ATK, defending_pokemon_attack_spinbox->value());
    selected_pokemon.setEV(Stats::SPATK, defending_pokemon_spattack_spinbox->value());
    selected_pokemon.setEV(Stats::SPE, defending_pokemon_speed_spinbox->value());

    std::vector<float> rolls;
    auto result = selected_pokemon.resistMove(turns, modifiers, rolls);

    result_window->setModal(true);
    result_window->setResult(selected_pokemon, turns, result, rolls);
    result_window->show();
}
