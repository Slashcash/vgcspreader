#include "resultwindow.hpp"
#include "mainwindow.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>

ResultWindow::ResultWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    result_pokemon = new Pokemon(1); //just random assignment

    QHBoxLayout* main_layout = new QHBoxLayout;

    setLayout(main_layout);

    createResultGroupBox();
    main_layout->addWidget(result_groupbox);

    createCalcGroupBox();
    main_layout->addWidget(calc_groupbox);

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void ResultWindow::createResultGroupBox() {
    result_groupbox = new QGroupBox("Result:");

    QVBoxLayout* result_layout = new QVBoxLayout;
    result_layout->setAlignment(Qt::AlignTop);

    //result groupbox
    result_groupbox->setLayout(result_layout);

    QComboBox* result_type_combobox = new QComboBox;
    result_type_combobox->setObjectName("result_type_combobox");
    result_layout->addWidget(result_type_combobox);

    QLabel* sprite = new QLabel;
    sprite->setObjectName("sprite");
    sprite->setAlignment(Qt::AlignCenter);
    result_layout->addWidget(sprite);

    QLabel* hp_evs = new QLabel(tr("HP EVS: ")+"0");
    hp_evs->setObjectName("hp_evs");
    result_layout->addWidget(hp_evs);

    QLabel* atk_evs = new QLabel(tr("Attack EVS: ")+"0");
    atk_evs->setObjectName("atk_evs");
    result_layout->addWidget(atk_evs);

    QLabel* def_evs = new QLabel(tr("Defense EVS: ")+"0");
    def_evs->setObjectName("def_evs");
    result_layout->addWidget(def_evs);

    QLabel* spatk_evs = new QLabel(tr("Sp. Attack EVS: ")+"0");
    spatk_evs->setObjectName("spatk_evs");
    result_layout->addWidget(spatk_evs);

    QLabel* spdef_evs = new QLabel(tr("Sp. Defense EVS: ")+"0");
    spdef_evs->setObjectName("spdef_evs");
    result_layout->addWidget(spdef_evs);

    QLabel* spe_evs = new QLabel(tr("Speed EVS: ")+"0");
    spe_evs->setObjectName("spe_evs");
    result_layout->addWidget(spe_evs);

    result_layout->addSpacing(15);

    QLabel* remaining_evs = new QLabel(tr("Remaining EVS: ")+"0");
    remaining_evs->setObjectName("remaining_evs");
    result_layout->addWidget(remaining_evs);

    QLabel* no_spread_label = new QLabel(tr("Sorry, no such spread exists"));
    no_spread_label->setObjectName("no_spread_label");
    no_spread_label->setVisible(false);
    result_layout->addWidget(no_spread_label);

    QLabel* no_input_label = new QLabel(tr("Input some attacks first"));
    no_input_label->setObjectName("no_input_label");
    no_input_label->setVisible(false);
    result_layout->addWidget(no_input_label);

    //connecting signals
    connect(result_type_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setResultType(int)));

}

void ResultWindow::createCalcGroupBox() {
    QVBoxLayout* calc_layout = new QVBoxLayout;
    calc_groupbox = new QGroupBox("Calcs:");
    calc_groupbox->setLayout(calc_layout);

    QTabWidget* result_tab = new QTabWidget;
    result_tab->setObjectName("result_tab");

    //defense
    QVBoxLayout* defense_tab_layout = new QVBoxLayout;

    //apparently we need this shit? (https://stackoverflow.com/questions/41520627/qt-how-to-add-multiple-qgroupbox-to-a-qtabwidget)
    QWidget* temp_1 = new QWidget;
    temp_1->setLayout(defense_tab_layout);

    QTextEdit* text_edit_defense = new QTextEdit;
    text_edit_defense->setObjectName("text_edit_defense");
    text_edit_defense->setReadOnly(true);
    defense_tab_layout->addWidget(text_edit_defense);

    QLabel* noattacks_defense_label = new QLabel(tr("Input some attacks to get calculations here!"));
    noattacks_defense_label->setObjectName("noattacks_defense_label");
    noattacks_defense_label->setVisible(false);
    defense_tab_layout->addWidget(noattacks_defense_label);

    result_tab->addTab(temp_1, tr("Defense"));

    //attack
    QVBoxLayout* attack_tab_layout = new QVBoxLayout;

    //apparently we need this shit? (https://stackoverflow.com/questions/41520627/qt-how-to-add-multiple-qgroupbox-to-a-qtabwidget)
    QWidget* temp_2 = new QWidget;
    temp_2->setLayout(attack_tab_layout);

    QTextEdit* text_edit_attack = new QTextEdit;
    text_edit_attack->setObjectName("text_edit_attack");
    text_edit_attack->setReadOnly(true);
    attack_tab_layout->addWidget(text_edit_attack);

    QLabel* noattacks_attack_label = new QLabel(tr("Input some attacks to get calculations here!"));
    noattacks_attack_label->setObjectName("noattacks_attack_label");
    noattacks_attack_label->setVisible(false);
    attack_tab_layout->addWidget(noattacks_attack_label);

    result_tab->addTab(temp_2, tr("Attack"));

    calc_layout->addWidget(result_tab);
}

void ResultWindow::setResult(const Pokemon& thePokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<attack_modifier>& theAtkModifier, const std::vector<Turn>& theDefendTurns, const std::vector<Turn>& theAttackTurns, const std::vector<Pokemon>& theDefPokemonInAtk, const DefenseResult theDefenseResult, const AttackResult theAttackResult) {
    //if no valid spread exists
    if( theDefenseResult.isEmpty() || theAttackResult.isEmpty() ) {
        result_groupbox->findChild<QComboBox*>("result_type_combobox")->setVisible(false);
        result_groupbox->findChild<QLabel*>("hp_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("def_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("spdef_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("atk_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("spatk_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("spe_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("remaining_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("no_spread_label")->setVisible(true);
        result_groupbox->findChild<QLabel*>("no_input_label")->setVisible(false);
        result_groupbox->findChild<QLabel*>("sprite")->setVisible(false);

        calc_groupbox->setVisible(false);
    }

    //if there is no input
    else if( theDefenseResult.isEmptyInput() && theAttackResult.isEmptyInput() ) {
        result_groupbox->findChild<QComboBox*>("result_type_combobox")->setVisible(false);
        result_groupbox->findChild<QLabel*>("hp_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("def_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("spdef_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("atk_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("spatk_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("spe_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("remaining_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("no_spread_label")->setVisible(false);
        result_groupbox->findChild<QLabel*>("no_input_label")->setVisible(true);
        result_groupbox->findChild<QLabel*>("sprite")->setVisible(false);

        calc_groupbox->setVisible(false);
    }

    //if the calculation is correct
    else {
        calc_groupbox->findChild<QTabWidget*>("result_tab")->setCurrentIndex(0);

        result_groupbox->findChild<QComboBox*>("result_type_combobox")->setVisible(true);
        result_groupbox->findChild<QLabel*>("hp_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("def_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("spdef_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("atk_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("spatk_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("spe_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("remaining_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("no_spread_label")->setVisible(false);
        result_groupbox->findChild<QLabel*>("no_input_label")->setVisible(false);
        result_groupbox->findChild<QLabel*>("sprite")->setVisible(true);

        calc_groupbox->setVisible(true);
        calc_groupbox->findChild<QTextEdit*>("text_edit_defense")->clear();
        calc_groupbox->findChild<QTextEdit*>("text_edit_attack")->clear();
        result_groupbox->findChild<QComboBox*>("result_type_combobox")->clear();

        //filling the member variables
        *result_pokemon = thePokemon;
        result_def_modifier = theDefModifier;
        result_atk_modifier = theAtkModifier;
        result_def_turns = theDefendTurns;
        result_atk_turns = theAttackTurns;
        result_pkmn_attack = theDefPokemonInAtk;
        result_def = theDefenseResult;
        result_atk = theAttackResult;

        //constructing the result type combobox
        if( theDefenseResult.hp_ev.size() == 1 ) result_groupbox->findChild<QComboBox*>("result_type_combobox")->setVisible(false);

        else {
            for( unsigned int i = 0; i < theDefenseResult.hp_ev.size(); i++ ) {
                if( i == 0 ) result_groupbox->findChild<QComboBox*>("result_type_combobox")->addItem(tr("Most Efficient Spread"));
                if( i == 1 ) result_groupbox->findChild<QComboBox*>("result_type_combobox")->addItem(tr("HP Balanced Spread"));
            }

            result_groupbox->findChild<QComboBox*>("result_type_combobox")->setCurrentIndex(0);

        }

        setResultType(0);

    }
}

void ResultWindow::setResultDefense(const Pokemon& theDefendingPokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<Turn>& theTurns, const DefenseResult theResult, const int index) {
    if( theResult.isEmptyInput()  ) {
        calc_groupbox->findChild<QTextEdit*>("text_edit_defense")->setVisible(false);
        calc_groupbox->findChild<QLabel*>("noattacks_defense_label")->setVisible(true);
    }

    else {
        calc_groupbox->findChild<QTextEdit*>("text_edit_defense")->setVisible(true);
        calc_groupbox->findChild<QLabel*>("noattacks_defense_label")->setVisible(false);

        for( unsigned int it = 0; it < theTurns.size(); it++ ) {
            Pokemon pokemon_buffer = theDefendingPokemon;

            pokemon_buffer.setEV(Stats::HP, theResult.hp_ev[index]);
            pokemon_buffer.setEV(Stats::DEF, theResult.def_ev[index]);
            pokemon_buffer.setEV(Stats::SPDEF, theResult.spdef_ev[index]);
            pokemon_buffer.setModifier(Stats::DEF, std::get<1>(theDefModifier[it]));
            pokemon_buffer.setModifier(Stats::SPDEF, std::get<2>(theDefModifier[it]));
            QString final_result = getCompleteString(theTurns[it], pokemon_buffer, true, theResult.def_ko_prob[index][it], theResult.def_damage_perc[index][it]);
            calc_groupbox->findChild<QTextEdit*>("text_edit_defense")->setText(calc_groupbox->findChild<QTextEdit*>("text_edit_defense")->toPlainText() + final_result + "\n\n");

        }
    }
}

void ResultWindow::setResultAttack(const Pokemon& theAttackingPokemon, const std::vector<Pokemon>& theDefendingPokemon, const std::vector<attack_modifier>& theAtkModifier, const std::vector<Turn>& theTurns, const AttackResult theResult, const int index) {
    if( theResult.isEmptyInput()  ) {
        calc_groupbox->findChild<QTextEdit*>("text_edit_attack")->setVisible(false);
        calc_groupbox->findChild<QLabel*>("noattacks_attack_label")->setVisible(true);
    }

    else {
        calc_groupbox->findChild<QTextEdit*>("text_edit_attack")->setVisible(true);
        calc_groupbox->findChild<QLabel*>("noattacks_attack_label")->setVisible(false);

        for( unsigned int it = 0; it < theTurns.size(); it++ ) {
            //we need this switch because of the dual defensive/offensive nature of the Turn class
            Pokemon pokemon_buffer = theAttackingPokemon;
            pokemon_buffer.setEV(Stats::ATK, theResult.atk_ev[index]);
            pokemon_buffer.setEV(Stats::SPATK, theResult.spatk_ev[index]);
            pokemon_buffer.setModifier(Stats::ATK, theAtkModifier[it].first);
            pokemon_buffer.setModifier(Stats::SPATK, theAtkModifier[it].second);

            std::vector<Turn> temp_turns = theTurns;
            Turn temp_turn;
            temp_turn.setHits(temp_turns[it].getHits());
            temp_turn.addMove(pokemon_buffer, temp_turns[it].getMoves()[0].second);

            temp_turns[it] = temp_turn;

            QString final_result = getCompleteString(temp_turns[it], theDefendingPokemon[it], false, theResult.atk_ko_prob[index][it], theResult.atk_damage_perc[index][it]);
            calc_groupbox->findChild<QTextEdit*>("text_edit_attack")->setText(calc_groupbox->findChild<QTextEdit*>("text_edit_attack")->toPlainText() + final_result + "\n\n");
        }
    }
}


QString ResultWindow::getAttackPokemon(const Pokemon& thePokemon, const Move& theMove) {
    QString atk_evs;
    if( theMove.getMoveCategory() == Move::Category::SPECIAL ) {
        if( thePokemon.getModifier(Stats::SPATK) != 0 ) {
            if( thePokemon.getModifier(Stats::SPATK) > 0 ) atk_evs = atk_evs + "+";
            atk_evs = atk_evs + QString::number(thePokemon.getModifier(Stats::SPATK)) + " ";
        }

        atk_evs = atk_evs + QString::number(thePokemon.getEV(Stats::SPATK));
        if( thePokemon.getNature() == Stats::MODEST || thePokemon.getNature() == Stats::QUIET|| thePokemon.getNature() == Stats::MILD || thePokemon.getNature() == Stats::RASH ) atk_evs = atk_evs + "+ SpA ";
        else if( thePokemon.getNature() == Stats::ADAMANT || thePokemon.getNature() == Stats::IMPISH || thePokemon.getNature() == Stats::JOLLY || thePokemon.getNature() == Stats::CAREFUL ) atk_evs = atk_evs + "- SpA ";
        else atk_evs = atk_evs + " SpA ";
    }

    else {
        if( thePokemon.getModifier(Stats::ATK) != 0 ) {
            if( thePokemon.getModifier(Stats::ATK) > 0 ) atk_evs = atk_evs + "+";
            atk_evs = atk_evs + QString::number(thePokemon.getModifier(Stats::ATK)) + " ";
        }

        atk_evs = atk_evs + QString::number(thePokemon.getEV(Stats::ATK));
        if( thePokemon.getNature() == Stats::LONELY || thePokemon.getNature() == Stats::BRAVE || thePokemon.getNature() == Stats::ADAMANT || thePokemon.getNature() == Stats::NAUGHTY ) atk_evs = atk_evs + "+ Atk ";
        else if( thePokemon.getNature() == Stats::BOLD || thePokemon.getNature() == Stats::TIMID || thePokemon.getNature() == Stats::MODEST || thePokemon.getNature() == Stats::CALM ) atk_evs = atk_evs + "- Atk ";
        else atk_evs = atk_evs + " Atk ";
    }

    //atk1_item
    QString atk_item;
    if( thePokemon.getItem().getIndex() != Items::None ) atk_item = atk_item + ((MainWindow*)parentWidget())->getItemsNames()[thePokemon.getItem().getIndex()] + " ";

    //atk1_ability
    QString atk_ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[thePokemon.getAbility()] + " ";

    //atk1_pokemon
    QString atk_pokemon = ((MainWindow*)parentWidget())->getSpeciesNames()[thePokemon.getPokedexNumber()-1] + " ";

    //atk1_move
    QString atk_move;
    if( theMove.isZ() ) atk_move = atk_move + "Z-";
    atk_move = atk_move + ((MainWindow*)parentWidget())->getMovesNames()[theMove.getMoveIndex()] + " ";

    //atk1 crit
    QString atk_crit;
    if( theMove.isCrit() ) atk_crit = atk_crit + "on a critical hit ";

    return atk_evs + atk_item + atk_ability + atk_pokemon + atk_move + atk_crit;
}

QString ResultWindow::getDefendPokemon(const Pokemon& thePokemon, const std::tuple<int, int, int>& theResult, const defense_modifier& theDefModifier, const Move& theMove, const bool isDualDefense) {
    //hp
    QString hp_evs = QString::number(std::get<0>(theResult)) + " HP";

    //def
    QString def_evs;
    def_evs = def_evs + " / ";
    if(theMove.getMoveCategory() == Move::PHYSICAL || isDualDefense ) {
        if( std::get<1>(theDefModifier) != 0 ) {
            if( std::get<1>(theDefModifier) > 0 ) def_evs = def_evs + "+";
            def_evs = def_evs + QString::number(std::get<1>(theDefModifier)) + " ";
        }

        def_evs = def_evs + QString::number(std::get<1>(theResult));

        if( thePokemon.getNature() == Stats::BOLD || thePokemon.getNature() == Stats::RELAXED || thePokemon.getNature() == Stats::IMPISH || thePokemon.getNature() == Stats::LAX ) def_evs = def_evs + "+ Def";
        else if( thePokemon.getNature() == Stats::HASTY || thePokemon.getNature() == Stats::MILD || thePokemon.getNature() == Stats::LONELY || thePokemon.getNature() == Stats::GENTLE ) def_evs = def_evs + "- Def";
        else def_evs = def_evs + " Def";
    }

    //spdef
    QString spdef_evs;
    spdef_evs = spdef_evs + " / ";
    if( theMove.getMoveCategory() == Move::SPECIAL || isDualDefense ) {
        if( std::get<2>(theDefModifier) != 0 ) {
            if( std::get<2>(theDefModifier) > 0 ) spdef_evs = spdef_evs + "+";
            spdef_evs = spdef_evs + QString::number(std::get<2>(theDefModifier)) + " ";
        }

         spdef_evs = spdef_evs + QString::number(std::get<2>(theResult));

        if( thePokemon.getNature() == Stats::CALM || thePokemon.getNature() == Stats::GENTLE || thePokemon.getNature() == Stats::SASSY || thePokemon.getNature() == Stats::CAREFUL ) spdef_evs = spdef_evs + "+ SpD ";
        else if( thePokemon.getNature() == Stats::NAUGHTY || thePokemon.getNature() == Stats::LAX || thePokemon.getNature() == Stats::NAIVE || thePokemon.getNature() == Stats::RASH ) spdef_evs = spdef_evs + "- SpD ";
        else spdef_evs = spdef_evs + " SpD ";
    }

    //item
    QString item;
    if( thePokemon.getItem().getIndex() != Items::None ) item = ((MainWindow*)parentWidget())->getItemsNames()[thePokemon.getItem().getIndex()] + " ";

    //ability
    QString ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[thePokemon.getAbility()] + " ";

    //species
    QString species = ((MainWindow*)parentWidget())->getSpeciesNames()[thePokemon.getPokedexNumber()-1] + " ";

    return hp_evs + def_evs + spdef_evs + item + ability + species;
}

QString ResultWindow::getModifiers(const Turn& theTurn) {
    QString modifier_result;

    //rain
    bool in_rain = false;
    bool rain_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getWeather() == Move::Weather::RAIN ) in_rain = true;
        if( it2->second.getMoveType() == Type::Fire || it2->second.getMoveType() == Type::Water ) rain_affected_move = true;
    }

    if( in_rain && rain_affected_move ) modifier_result = modifier_result + "in Rain ";

    //sun
    bool in_sun = false;
    bool sun_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getWeather() == Move::Weather::SUN ) in_sun = true;
        if( it2->second.getMoveType() == Type::Fire || it2->second.getMoveType() == Type::Water ) sun_affected_move = true;
    }

    if( in_sun && sun_affected_move ) modifier_result = modifier_result + "in Sun ";

    //harsh sun
    bool in_harsh_sun = false;
    sun_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getWeather() == Move::Weather::HARSH_SUNSHINE ) in_harsh_sun = true;
        if( it2->second.getMoveType() == Type::Fire || it2->second.getMoveType() == Type::Water ) sun_affected_move = true;
    }

    if( in_harsh_sun && sun_affected_move ) modifier_result = modifier_result + "in Harsh Sun ";

    //heavy rain
    bool in_heavy_rain = false;
    rain_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getWeather() == Move::Weather::HEAVY_RAIN ) in_heavy_rain = true;
        if( it2->second.getMoveType() == Type::Fire || it2->second.getMoveType() == Type::Water ) rain_affected_move = true;
    }

    if( in_heavy_rain && rain_affected_move ) modifier_result = modifier_result + "in Heavy Rain ";

    //electric
    bool in_electric = false;
    bool electric_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getTerrain() == Move::Terrain::ELECTRIC ) in_electric = true;
        if( it2->second.getMoveType() == Type::Electric && it2->first.isGrounded() ) electric_affected_move = true;
    }

    if( in_electric && electric_affected_move ) modifier_result = modifier_result + "in Electric Terrain ";

    //psychic
    bool in_psychic = false;
    bool psychic_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getTerrain() == Move::Terrain::PSYCHIC ) in_psychic = true;
        if( it2->second.getMoveType() == Type::Psychic && it2->first.isGrounded() ) psychic_affected_move = true;
    }

    if( in_psychic && psychic_affected_move ) modifier_result = modifier_result + "in Psychic Terrain ";

    //grassy
    bool in_grassy = false;
    bool grassy_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getTerrain() == Move::Terrain::GRASSY ) in_grassy = true;
        if( it2->second.getMoveType() == Type::Grass && it2->first.isGrounded() ) grassy_affected_move = true;
    }

    if( in_grassy && grassy_affected_move ) modifier_result = modifier_result + "in Grassy Terrain ";

    //misty
    bool in_misty = false;
    bool misty_affected_move = false;
    for( auto it2 = theTurn.getMoves().begin(); it2 < theTurn.getMoves().end(); it2++ ) {
        if( it2->second.getTerrain() == Move::Terrain::MISTY ) in_misty = true;
        if( it2->second.getMoveType() == Type::Dragon && it2->first.isGrounded() ) misty_affected_move = true;
    }

    if( in_misty && misty_affected_move ) modifier_result = modifier_result + "in Misty Terrain ";

    return modifier_result;
}

QString ResultWindow::getResults(const Turn& theTurn, const Pokemon& thePokemon, const float theRoll, const std::vector<float>& theDamagePerc) {
    //RESULT
    QString roll_result = "-- ";
    if(theTurn.getHits() > 1) roll_result = roll_result + QString::number(100-theRoll, 'f', 1) + "% chance of resisting " + QString::number(theTurn.getHits()) + " moves ";
    else roll_result = roll_result + QString::number(100-theRoll, 'f', 1) + "% chance of resisting " + QString::number(theTurn.getHits()) + " move ";

    //DAMAGE
    //doing this to match showdown's output format
    float min_damage;
    float max_damage;
    min_damage = *std::min_element(theDamagePerc.begin(), theDamagePerc.end());
    min_damage = std::floor(min_damage*10) / 10;

    max_damage = *std::max_element(theDamagePerc.begin(), theDamagePerc.end());
    max_damage = std::floor(max_damage*10) / 10;

    QString damage_result = "(" + QString::number(min_damage, 'f', 1) + "% - " + QString::number(max_damage, 'f', 1) + "%)" /*+ "\n("*/;

    //RESTORER
    //grassy terrain restore
    QString restore_result;
    bool grassy_terrain_recover = false;
    auto turn_moves = theTurn.getMoves();
    for( auto it_turn = turn_moves.begin(); it_turn < turn_moves.end(); it_turn++ )
        if( it_turn->second.getTerrain() == Move::Terrain::GRASSY && thePokemon.isGrounded() ) grassy_terrain_recover = true;

    if( grassy_terrain_recover ) restore_result = restore_result + " after Grassy Terrain recover ";

    return roll_result + damage_result + restore_result;
}

QString ResultWindow::getCompleteString(const Turn& theTurn, const Pokemon& theDefendingPokemon, const bool isDualDefense, const float theRoll, const std::vector<float>& theDamagePerc) {
    QString first_result;
    QString second_result;
    QString defender_result;
    QString modifier_result;
    QString damage_result;


    //FIRST POKEMON
    first_result = getAttackPokemon(theTurn.getMoves()[0].first, theTurn.getMoves()[0].second);

    //SECOND POKEMON
    if( theTurn.getMoveNum() > 1 ) {
        //SECOND POKEMON
        QString plus_sign = "[+] ";

        second_result = plus_sign + getAttackPokemon(theTurn.getMoves()[1].first, theTurn.getMoves()[1].second);
    }

    //DEFENDING POKEMON
    QString vs_string = "vs. ";
    defender_result = vs_string + getDefendPokemon(theDefendingPokemon, std::make_tuple(theDefendingPokemon.getEV(Stats::HP), theDefendingPokemon.getEV(Stats::DEF), theDefendingPokemon.getEV(Stats::SPDEF)), std::make_tuple(100, theDefendingPokemon.getModifier(Stats::DEF), theDefendingPokemon.getModifier(Stats::SPDEF)), theTurn.getMoves()[0].second, isDualDefense);

    //MODIFIERS
    modifier_result = getModifiers(theTurn);

    //RESULT
    damage_result = getResults(theTurn, theDefendingPokemon, theRoll, theDamagePerc);

    return first_result + second_result + defender_result + modifier_result + damage_result;
}

void ResultWindow::setResultType(int index) {
    if( index >= 0) { //to prevent the crash when clearing the combobox
        calc_groupbox->findChild<QTextEdit*>("text_edit_defense")->clear();
        calc_groupbox->findChild<QTextEdit*>("text_edit_attack")->clear();

        //setting correct sprite
        QPixmap sprite_pixmap;
        QString sprite_path;
        bool load_result;

        if( result_pokemon->getForm() == 0 ) sprite_path = ":/db/sprites/" + QString::number(result_pokemon->getPokedexNumber()) + ".png";
        else sprite_path = ":/db/sprites/" + QString::number(result_pokemon->getPokedexNumber()) + "-" + QString::number(result_pokemon->getForm()) + ".png";

        sprite_pixmap.load(sprite_path);
        const int SPRITE_SCALE_FACTOR = 2;
        sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

        QLabel* sprite = result_groupbox->findChild<QLabel*>("sprite");
        sprite->setPixmap(sprite_pixmap);

        int show_hp_evs;
        if( result_def.hp_ev[0] < 0 ) show_hp_evs = result_pokemon->getEV(Stats::HP);
        else show_hp_evs = result_def.hp_ev[index];
        result_groupbox->findChild<QLabel*>("hp_evs")->setText(tr("HP EVS: ")+QString::number(show_hp_evs));

        int show_atk_evs;
        if( result_atk.atk_ev[0] < 0 ) show_atk_evs = result_pokemon->getEV(Stats::ATK);
        else show_atk_evs = result_atk.atk_ev[index];
        result_groupbox->findChild<QLabel*>("atk_evs")->setText(tr("Attack EVS: ")+QString::number(show_atk_evs));

        int show_def_evs;
        if( result_def.def_ev[0] < 0 ) show_def_evs = result_pokemon->getEV(Stats::DEF);
        else show_def_evs = result_def.def_ev[index];
        result_groupbox->findChild<QLabel*>("def_evs")->setText(tr("Defense EVS: ")+QString::number(show_def_evs));

        int show_spatk_evs;
        if( result_atk.spatk_ev[0] < 0 ) show_spatk_evs = result_pokemon->getEV(Stats::SPATK);
        else show_spatk_evs = result_atk.spatk_ev[index];
        result_groupbox->findChild<QLabel*>("spatk_evs")->setText(tr("Sp. Attack EVS: ")+QString::number(show_spatk_evs));

        int show_spdef_evs;
        if( result_def.spdef_ev[0] < 0 ) show_spdef_evs = result_pokemon->getEV(Stats::SPDEF);
        else show_spdef_evs = result_def.spdef_ev[index];
        result_groupbox->findChild<QLabel*>("spdef_evs")->setText(tr("Sp. Defense EVS: ")+QString::number(show_spdef_evs));

        result_groupbox->findChild<QLabel*>("spe_evs")->setText(tr("Speed EVS: ")+QString::number(result_pokemon->getEV(Stats::SPE)));

        //calculating how many evs do remain to use
        int rem_hp = result_def.hp_ev[index];
        if( rem_hp < 0 ) rem_hp = result_pokemon->getEV(Stats::HP); //all of there are necessaries because in some special cases the returned result is < 0

        int rem_def = result_def.def_ev[index];
        if( rem_def < 0 ) rem_def = result_pokemon->getEV(Stats::DEF);

        int rem_spdef = result_def.spdef_ev[index];
        if( rem_spdef < 0 ) rem_spdef = result_pokemon->getEV(Stats::SPDEF);

        int rem_atk = result_atk.atk_ev[index];
        if( rem_atk < 0 ) rem_atk = result_pokemon->getEV(Stats::ATK);

        int rem_spatk = result_atk.spatk_ev[index];
        if( rem_spatk < 0 ) rem_spatk = result_pokemon->getEV(Stats::SPATK);

        int rem_spe = result_pokemon->getEV(Stats::SPE);
        if( rem_spe < 0 ) rem_spe = result_pokemon->getEV(Stats::SPE);

        result_groupbox->findChild<QLabel*>("remaining_evs")->setText(tr("Remaining EVS: ")+QString::number(508 - rem_hp - rem_def - rem_spdef - rem_atk - rem_spatk - rem_spe));

        setResultDefense(*result_pokemon, result_def_modifier, result_def_turns, result_def, index);
        setResultAttack(*result_pokemon, result_pkmn_attack, result_atk_modifier, result_atk_turns,result_atk, index);
    }
}
