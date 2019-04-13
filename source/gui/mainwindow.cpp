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
#include <QThread>
#include <QProgressBar>
#include <QTabWidget>
#include <qtconcurrentrun.h>

#include "pokemon.hpp"

MainWindow::MainWindow() {
    setObjectName("MainWindow");
    setWindowTitle("VGCSpreader");
    selected_pokemon = nullptr;

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

    move_window = new DefenseMoveWindow(this);
    move_window->setObjectName("MoveWindow");
    move_window->setWindowTitle("VGCSpreader");

    result_window = new ResultWindow(this);
    result_window->setObjectName("ResultWindow");
    result_window->setWindowTitle("VGCSpreader");

    alert_window = new AlertWindow(this);
    alert_window->setObjectName("ResultWindow");
    alert_window->setWindowTitle("VGCSpreader");

    //bottom buttons
    bottom_buttons = new QDialogButtonBox;
    QPushButton* calculate_button = new QPushButton(tr("Calculate"));
    calculate_button->setObjectName("calculate_button");
    QPushButton* clear_button = new QPushButton(tr("Clear"));
    clear_button->setObjectName("clear_button");
    QPushButton* stop_button = new QPushButton(tr("Stop"));
    stop_button->setObjectName("stop_button");

    bottom_buttons->addButton(clear_button, QDialogButtonBox::ButtonRole::ResetRole);
    bottom_buttons->addButton(calculate_button, QDialogButtonBox::ButtonRole::AcceptRole);
    bottom_buttons->addButton(stop_button, QDialogButtonBox::ButtonRole::ApplyRole);
    stop_button->setVisible(false);

    main_layout->addWidget(bottom_buttons, Qt::AlignRight);

    //SIGNAL
    connect(move_window->bottom_button_box, SIGNAL(accepted()), this, SLOT(solveMoveDefense()));
    connect(alert_window->bottom_buttons, SIGNAL(accepted()), this, SLOT(calculate()));
    connect(bottom_buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clear(QAbstractButton*)));
    connect(bottom_buttons, SIGNAL(accepted()), this, SLOT(calculateStart()));
    connect(&this->future_watcher, SIGNAL (finished()), this, SLOT (calculateFinished()));

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void MainWindow::moveTabChanged(int index) {
    moves_groupbox->findChild<QPushButton*>("moves_edit_button")->setEnabled(false);
    moves_groupbox->findChild<QPushButton*>("moves_delete_button")->setEnabled(false);

    moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setCurrentItem(nullptr);
    moves_groupbox->findChild<QTableWidget*>("moves_attack_view")->setCurrentItem(nullptr);

}

void MainWindow::setButtonClickable(int row, int column) {
    moves_groupbox->findChild<QPushButton*>("moves_edit_button")->setEnabled(true);
    moves_groupbox->findChild<QPushButton*>("moves_delete_button")->setEnabled(true);
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
    turns_def.erase(turns_def.begin() +  moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow());
    modifiers_def.erase(modifiers_def.begin() + moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow());

    moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->removeRow(moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow());

    if( turns_def.empty() ) {
        moves_groupbox->findChild<QPushButton*>("moves_edit_button")->setEnabled(false);
        moves_groupbox->findChild<QPushButton*>("moves_delete_button")->setEnabled(false);
    }
}

void MainWindow::createDefendingPokemonGroupBox() {
    defending_groupbox = new QGroupBox(tr("Pokemon:"));

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

    //some spacing
    species_types_layout->insertSpacing(5, 20);

    //---assigned evs---//
    QHBoxLayout* assigned_layout = new QHBoxLayout;
    assigned_layout->setAlignment(Qt::AlignLeft);
    assigned_layout->setSpacing(10);

    //already assigned evs
    QLabel* assignedev_label = new QLabel(tr("Already assigned EV:"));
    assigned_layout->addWidget(assignedev_label);

    QSpinBox* assignedev = new QSpinBox;
    assignedev->setObjectName("defending_assignedev_spinbox");
    assignedev->setRange(0, 508);
    assigned_layout->addWidget(assignedev);

    species_types_layout->addLayout(assigned_layout);

    //adding everything to the window
    defending_layout->addLayout(species_types_layout);

    //and then some spacing
    defending_layout->insertSpacing(1, 35);

    //---MAIN FORM---//
    QVBoxLayout* main_form_layout = new QVBoxLayout;
    main_form_layout->setAlignment(Qt::AlignTop);

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
    QHBoxLayout* modifiers_layout_1 = new QHBoxLayout;
    modifiers_layout_1->setAlignment(Qt::AlignLeft);
    modifiers_layout_1->setSpacing(10);

    //hp iv
    QLabel* hpiv_label = new QLabel(tr("HP IV:"));
    hpiv_label->setObjectName("defending_hpiv_label");
    modifiers_layout_1->addWidget(hpiv_label);

    QSpinBox* hpiv = new QSpinBox;
    hpiv->setObjectName("defending_hpiv_spinbox");
    hpiv->setRange(0, 31);
    hpiv->setValue(31);
    modifiers_layout_1->addWidget(hpiv);

    //def iv
    QLabel* defiv_label = new QLabel(tr("Def IV:"));
    defiv_label->setObjectName("defending_defiv_label");
    modifiers_layout_1->addWidget(defiv_label);

    QSpinBox* defiv = new QSpinBox;
    defiv->setRange(0, 31);
    defiv->setValue(31);
    defiv->setObjectName("defending_defiv_spinbox");
    modifiers_layout_1->addWidget(defiv);

    //spdef iv
    QLabel* defending_spdefiv_label = new QLabel(tr("Sp. Def IV:"));
    defending_spdefiv_label->setObjectName("defending_spdefiv_label");
    modifiers_layout_1->addWidget(defending_spdefiv_label);

    QSpinBox* spdefiv = new QSpinBox;
    spdefiv->setObjectName("defending_spdefiv_spinbox");
    spdefiv->setRange(0, 31);
    spdefiv->setValue(31);
    modifiers_layout_1->addWidget(spdefiv);

    QHBoxLayout* modifiers_layout_2 = new QHBoxLayout;
    modifiers_layout_2->setAlignment(Qt::AlignLeft);
    modifiers_layout_2->setSpacing(10);

    //atk iv
    QLabel* defending_atkiv_label = new QLabel(tr("Atk IV:"));
    defending_atkiv_label->setObjectName("defending_atkiv_label");
    modifiers_layout_2->addWidget(defending_atkiv_label);

    QSpinBox* atkiv = new QSpinBox;
    atkiv->setObjectName("defending_atkiv_spinbox");
    atkiv->setRange(0, 31);
    atkiv->setValue(31);
    modifiers_layout_2->addWidget(atkiv);

    //spatk iv
    QLabel* defending_spatkiv_label = new QLabel(tr("Sp. Atk IV:"));
    defending_spatkiv_label->setObjectName("defending_spatkiv_label");
    modifiers_layout_2->addWidget(defending_spatkiv_label);

    QSpinBox* spatkiv = new QSpinBox;
    spatkiv->setObjectName("defending_spatkiv_spinbox");
    spatkiv->setRange(0, 31);
    spatkiv->setValue(31);
    modifiers_layout_2->addWidget(spatkiv);

    main_form_layout->addLayout(modifiers_layout_1);
    main_form_layout->addLayout(modifiers_layout_2);

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

    QHBoxLayout* moves_layout = new QHBoxLayout;

    moves_groupbox->setLayout(moves_layout);

    //Buttons
    QVBoxLayout* button_layout = new QVBoxLayout;
    button_layout->setSpacing(5);
    button_layout->setAlignment(Qt::AlignTop);
    //MOVES ADD BUTTON
    QPushButton* moves_add_button = new QPushButton(tr("Add"));
    moves_add_button->setObjectName("moves_add_button");
    button_layout->addWidget(moves_add_button);

    //MOVES EDIT BUTTON
    QPushButton* moves_edit_button = new QPushButton(tr("Edit"));
    moves_edit_button->setObjectName("moves_edit_button");
    moves_edit_button->setEnabled(false);
    button_layout->addWidget(moves_edit_button);

    //MOVES DELETE BUTTON
    QPushButton* moves_delete_button = new QPushButton(tr("Delete"));
    moves_delete_button->setObjectName("moves_delete_button");
    moves_delete_button->setEnabled(false);
    button_layout->addWidget(moves_delete_button);

    moves_layout->addLayout(button_layout);

    //creating the two tabs for attack and defense
    QVBoxLayout* tab_layout = new QVBoxLayout;

    QTabWidget* tabs = new QTabWidget;
    tabs->setObjectName("moves_tabs");
    tab_layout->addWidget(tabs);

    //MOVES DEFENSE VIEW
    QTableWidget* moves_defense_view = new QTableWidget(0, 3);
    moves_defense_view->setObjectName("moves_defense_view");
    tabs->addTab(moves_defense_view, tr("Defense"));
    moves_defense_view->horizontalHeader()->setVisible(false);
    moves_defense_view->verticalHeader()->setVisible(false);
    moves_defense_view->setShowGrid(false);
    moves_defense_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    moves_defense_view->setColumnWidth(1, 20);

    //MOVES ATTACK VIEW
    QTableWidget* moves_attack_view = new QTableWidget(0, 3);
    moves_attack_view->setObjectName("moves_attack_view");
    tabs->addTab(moves_attack_view, tr("Attack"));
    moves_attack_view->horizontalHeader()->setVisible(false);
    moves_attack_view->verticalHeader()->setVisible(false);
    moves_attack_view->setShowGrid(false);
    moves_attack_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    moves_attack_view->setColumnWidth(1, 20);

    //PRIORITIZE COMBOBOX
    QComboBox* prioritize_combobox = new QComboBox;
    prioritize_combobox->setObjectName("prioritize_combobox");
    prioritize_combobox->addItem(tr("Prioritize Defense"));
    prioritize_combobox->addItem(tr("Prioritize Attack"));
    tab_layout->addWidget(prioritize_combobox);

    int prioritize_width = prioritize_combobox->minimumSizeHint().width();
    prioritize_combobox->setMaximumWidth(prioritize_width);

    //adding the calculate spread label
    QHBoxLayout* progress_bar_layout = new QHBoxLayout;
    tab_layout->addLayout(progress_bar_layout);

    QLabel* calculating_spread_label = new QLabel(tr("Calculating spread..."));
    calculating_spread_label->setObjectName("calculating_spread_label");
    calculating_spread_label->setVisible(false);
    calculating_spread_label->setAlignment(Qt::AlignRight);
    progress_bar_layout->addWidget(calculating_spread_label);

    QProgressBar* progress_bar = new QProgressBar;
    progress_bar->setObjectName("progress_bar");
    progress_bar->setMinimum(0);
    progress_bar->setMaximum(0);
    progress_bar->setMaximumWidth(60);
    progress_bar_layout->addWidget(progress_bar);
    progress_bar->setVisible(false);

    moves_layout->addLayout(tab_layout);

    //CONNECTING SIGNALS
    connect(moves_defense_view, SIGNAL(cellClicked(int,int)), this, SLOT(setButtonClickable(int,int)));
    connect(moves_attack_view, SIGNAL(cellClicked(int,int)), this, SLOT(setButtonClickable(int,int)));
    connect(moves_delete_button, SIGNAL(clicked(bool)), this, SLOT(eraseMove(bool)));
    connect(moves_add_button, SIGNAL(clicked(bool)), this, SLOT(openMoveWindow(bool)));
    connect(moves_edit_button, SIGNAL(clicked(bool)), this, SLOT(openMoveWindowEdit(bool)));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(moveTabChanged(int)));
}

void MainWindow::solveMoveDefense() {
    if( !move_window->isEditMode() ) {
        moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setRowCount(moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->rowCount()+1);

        auto buffer = turns_def.back().getMoves();

        QString move_name_1;
        if( buffer[0].second.isZ() ) move_name_1 = tr("Z-") + moves_names[buffer[0].second.getMoveIndex()];
        else move_name_1 = moves_names[buffer[0].second.getMoveIndex()];
        QString move1(species_names[buffer[0].first.getPokedexNumber()-1] + " " + move_name_1);

       moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setItem(turns_def.size()-1, 0, new QTableWidgetItem(move1));

        moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->resizeColumnToContents(0);

        if( turns_def.back().getMoveNum() > 1 ) {
            QString move_name_2;
            if( buffer[1].second.isZ() ) move_name_2 = tr("Z-") + moves_names[buffer[1].second.getMoveIndex()];
            else move_name_2 = moves_names[buffer[1].second.getMoveIndex()];

            QString move2(species_names[buffer[1].first.getPokedexNumber()-1] + " " + move_name_2);

            QTableWidgetItem* plus_sign = new QTableWidgetItem("+");
            plus_sign->setTextAlignment(Qt::AlignCenter);
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setItem(turns_def.size()-1, 1, plus_sign);
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setItem(turns_def.size()-1, 2, new QTableWidgetItem(move2));
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->resizeColumnToContents(2);
        }
    }

    else {
        auto buffer = turns_def[moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow()].getMoves();

        QString move_name_1;
        if( buffer[0].second.isZ() ) move_name_1 = tr("Z-") + moves_names[buffer[0].second.getMoveIndex()];
        else move_name_1 = moves_names[buffer[0].second.getMoveIndex()];
        QString move1(species_names[buffer[0].first.getPokedexNumber()-1] + " " + move_name_1);

        moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setItem(moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow(), 0, new QTableWidgetItem(move1));

        moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->resizeColumnToContents(0);

        if( turns_def[moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow()].getMoveNum() > 1 ) {
            QString move_name_2;
            if( buffer[1].second.isZ() ) move_name_2 = tr("Z-") + moves_names[buffer[1].second.getMoveIndex()];
            else move_name_2 = moves_names[buffer[1].second.getMoveIndex()];

            QString move2(species_names[buffer[1].first.getPokedexNumber()-1] + " " + move_name_2);

            QTableWidgetItem* plus_sign = new QTableWidgetItem("+");
            plus_sign->setTextAlignment(Qt::AlignCenter);
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setItem(moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow(), 1, plus_sign);
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->setItem(moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow(), 2, new QTableWidgetItem(move2));
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->resizeColumnToContents(2);
        }

        else {
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->takeItem(moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow(), 1);
            moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->takeItem(moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow(), 2);
        }
    }
}

void MainWindow::openMoveWindow(bool checked) {
    //if the defensive window is requested
    if( moves_groupbox->findChild<QTabWidget*>("moves_tabs")->currentIndex() == 0 ) openMoveWindowDefense();

    //if the attacking window is requested
    if( moves_groupbox->findChild<QTabWidget*>("moves_tabs")->currentIndex() == 1 ) {
        //temporary test
        moves_groupbox->findChild<QTableWidget*>("moves_attack_view")->setRowCount(1);
        moves_groupbox->findChild<QTableWidget*>("moves_attack_view")->setItem(0, 0, new QTableWidgetItem("Schyter Precipice Blades"));

        //until here
    }
}

void MainWindow::openMoveWindowEdit(bool checked) {
    openMoveWindowEditDefense();
}

void MainWindow::openMoveWindowDefense() {
    move_window->setAsBlank();
    move_window->setEditMode(false);
    move_window->setModal(true);
    move_window->setVisible(true);
}

void MainWindow::openMoveWindowEditDefense() {
    move_window->setAsBlank();
    move_window->setAsTurn(turns_def[moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow()], modifiers_def[moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow()]);
    move_window->setEditMode(true);
    move_window->setModal(true);
    move_window->setVisible(true);
}

void MainWindow::addTurn(const Turn& theTurn, const defense_modifier& theModifier) {
    if( !move_window->isEditMode() ) {
        turns_def.push_back(theTurn);
        modifiers_def.push_back(theModifier);
    }

    else {
        turns_def[ moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow()] = theTurn;
        modifiers_def[ moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->currentRow()] = theModifier;
    }
}

void MainWindow::clear(QAbstractButton* theButton) {
    if( theButton->objectName() == "clear_button" ) {
        defending_groupbox->findChild<QComboBox*>("defending_species_combobox")->setCurrentIndex(0);
        defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->setCurrentIndex(0);
        defending_groupbox->findChild<QComboBox*>("defending_nature_combobox")->setCurrentIndex(0);
        defending_groupbox->findChild<QComboBox*>("defending_items_combobox")->setCurrentIndex(0);
        defending_groupbox->findChild<QSpinBox*>("defending_hpiv_spinbox")->setValue(31);
        defending_groupbox->findChild<QSpinBox*>("defending_defiv_spinbox")->setValue(31);
        defending_groupbox->findChild<QSpinBox*>("defending_spdefiv_spinbox")->setValue(31);
        defending_groupbox->findChild<QSpinBox*>("defending_assignedev_spinbox")->setValue(0);

        moves_groupbox->findChild<QPushButton*>("moves_edit_button")->setEnabled(false);
        moves_groupbox->findChild<QPushButton*>("moves_delete_button")->setEnabled(false);

        moves_groupbox->findChild<QTableWidget*>("moves_defense_view")->clear();
        moves_groupbox->findChild<QTableWidget*>("moves_attack_view")->clear();
        turns_def.clear();
        modifiers_def.clear();
        turns_atk.clear();
        modifiers_atk.clear();
        defending_pokemons_in_attack.clear();
    }

    else if( theButton->objectName() == "stop_button" ) calculateStop();
}

void MainWindow::calculate() {
    selected_pokemon = new Pokemon(defending_groupbox->findChild<QComboBox*>("defending_species_combobox")->currentIndex()+1);
    selected_pokemon->setForm(defending_groupbox->findChild<QComboBox*>("defending_forms_combobox")->currentIndex());
    selected_pokemon->setType(0, (Type)defending_groupbox->findChild<QComboBox*>("defending_type1_combobox")->currentIndex());
    selected_pokemon->setType(1, (Type)defending_groupbox->findChild<QComboBox*>("defending_type2_combobox")->currentIndex());
    selected_pokemon->setNature((Stats::Nature)defending_groupbox->findChild<QComboBox*>("defending_nature_combobox")->currentIndex());
    selected_pokemon->setAbility((Ability)defending_groupbox->findChild<QComboBox*>("defending_abilities_combobox")->currentIndex());
    selected_pokemon->setItem(Item(defending_groupbox->findChild<QComboBox*>("defending_items_combobox")->currentIndex()));
    selected_pokemon->setIV(Stats::HP, defending_groupbox->findChild<QSpinBox*>("defending_hpiv_spinbox")->value());
    selected_pokemon->setIV(Stats::DEF, defending_groupbox->findChild<QSpinBox*>("defending_defiv_spinbox")->value());
    selected_pokemon->setIV(Stats::SPDEF, defending_groupbox->findChild<QSpinBox*>("defending_spdefiv_spinbox")->value());
    selected_pokemon->setEV(Stats::SPE, defending_groupbox->findChild<QSpinBox*>("defending_assignedev_spinbox")->value());

    //enabling the stop button
    bottom_buttons->findChild<QPushButton*>("stop_button")->setVisible(true);
    //disabling other buttons
    bottom_buttons->findChild<QPushButton*>("calculate_button")->setEnabled(false);
    bottom_buttons->findChild<QPushButton*>("clear_button")->setEnabled(false);
    //enabling the progress bar
    moves_groupbox->findChild<QLabel*>("calculating_spread_label")->setVisible(true);
    moves_groupbox->findChild<QProgressBar*>("progress_bar")->setVisible(true);


    future = QtConcurrent::run(selected_pokemon, &Pokemon::resistMove, turns_def, modifiers_def);
    future_watcher.setFuture(future);
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

void MainWindow::calculateFinished() {
    DefenseResult result = future.result();

    if( !result.isAborted()  ) { //if an abort has been requested we don't show the result window
        result_window->setModal(true);

        //this part here is pure test
        modifiers_atk.push_back(std::make_pair(0, 0));

        Turn test_turn;
        test_turn.addMove(Pokemon(100), Move(Moves::Precipice_Blades));
        turns_atk.push_back(test_turn);

        defending_pokemons_in_attack.push_back(Pokemon(2));

        AttackResult test_result;
        test_result.atk_ev = 2;
        test_result.spatk_ev = 1;
        test_result.atk_ko_prob.push_back(100);

        std::vector<float> test_float;
        test_float.push_back(10);
        test_float.push_back(15);
        test_result.atk_damage_perc.push_back(test_float);
        //until here

        result_window->setResult(*selected_pokemon, modifiers_def, modifiers_atk, turns_def, turns_atk, defending_pokemons_in_attack, result, test_result);
        result_window->show();
    }
    delete selected_pokemon;

    //enabling/disabling buttons back
    bottom_buttons->findChild<QPushButton*>("calculate_button")->setEnabled(true);
    bottom_buttons->findChild<QPushButton*>("clear_button")->setEnabled(true);
    bottom_buttons->findChild<QPushButton*>("stop_button")->setVisible(false);
    moves_groupbox->findChild<QLabel*>("calculating_spread_label")->setVisible(false);
    moves_groupbox->findChild<QProgressBar*>("progress_bar")->setVisible(false);

}

void MainWindow::calculateStop() {
    selected_pokemon->abortCalculation();
}

void MainWindow::reject() {
    calculateStop(); //just in case a calculation is still in process
    QDialog::reject();
}

void MainWindow::calculateStart() {
    bool prompt = false;

    //checking if we have to show the prompt for the long calculation
    for( auto it = turns_def.begin(); it < turns_def.end(); it++ )
        if( it->getMoveNum() > 1 && it->getHits() > 2 )
            prompt = true;

    if( prompt ) {
        alert_window->setModal(true);
        alert_window->setVisible(true);
    }

    else calculate();
}
