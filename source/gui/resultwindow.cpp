#include "resultwindow.hpp"
#include "mainwindow.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

ResultWindow::ResultWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    QHBoxLayout* main_layout = new QHBoxLayout;

    setLayout(main_layout);

    createResultGroupBox();
    main_layout->addWidget(result_groupbox);

    QVBoxLayout* calc_layout = new QVBoxLayout;
    calc_groupbox = new QGroupBox("Calcs:");
    calc_groupbox->setLayout(calc_layout);
    QTextEdit* text_edit = new QTextEdit;
    text_edit->setObjectName("text_edit");
    text_edit->setReadOnly(true);
    calc_layout->addWidget(text_edit);
    main_layout->addWidget(calc_groupbox);

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void ResultWindow::createResultGroupBox() {
    result_groupbox = new QGroupBox("Result:");

    QVBoxLayout* result_layout = new QVBoxLayout;

    //result groupbox
    result_groupbox->setLayout(result_layout);

    QLabel* sprite = new QLabel;
    sprite->setObjectName("sprite");
    sprite->setAlignment(Qt::AlignCenter);
    result_layout->addWidget(sprite);

    QLabel* hp_evs = new QLabel(tr("HP EVS: ")+"0");
    hp_evs->setObjectName("hp_evs");
    result_layout->addWidget(hp_evs);

    QLabel* def_evs = new QLabel(tr("Defense EVS: ")+"0");
    def_evs->setObjectName("def_evs");
    result_layout->addWidget(def_evs);

    QLabel* spdef_evs = new QLabel(tr("Sp. Defense EVS: ")+"0");
    spdef_evs->setObjectName("spdef_evs");
    result_layout->addWidget(spdef_evs);
}

void ResultWindow::setResult(const Pokemon& theDefendingPokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<Turn>& theTurns, const std::tuple<int, int, int>& theResult, const std::vector<std::vector<float>>& theDamagePerc, const std::vector<float>& theRoll) {
    if( std::get<0>(theResult) == -1 || std::get<1>(theResult) == -1 || std::get<2>(theResult) == -1  ) {
        result_groupbox->findChild<QLabel*>("def_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("spdef_evs")->setVisible(false);
        result_groupbox->findChild<QLabel*>("sprite")->setVisible(false);

        result_groupbox->findChild<QLabel*>("hp_evs")->setText(tr("Sorry, no such spread exists"));
        calc_groupbox->setVisible(false);
    }

    else {
        result_groupbox->findChild<QLabel*>("hp_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("def_evs")->setVisible(true);
        result_groupbox->findChild<QLabel*>("spdef_evs")->setVisible(true);
        calc_groupbox->findChild<QTextEdit*>("text_edit")->setVisible(true);
        calc_groupbox->findChild<QTextEdit*>("text_edit")->clear();

        //setting correct sprite
        QPixmap sprite_pixmap;
        QString sprite_path;
        bool load_result;

        if( theDefendingPokemon.getForm() == 0 ) sprite_path = ":/db/sprites/" + QString::number(theDefendingPokemon.getPokedexNumber()) + ".png";
        else sprite_path = ":/db/sprites/" + QString::number(theDefendingPokemon.getPokedexNumber()) + "-" + QString::number(theDefendingPokemon.getForm()) + ".png";

        sprite_pixmap.load(sprite_path);
        const int SPRITE_SCALE_FACTOR = 2;
        sprite_pixmap = sprite_pixmap.scaled(sprite_pixmap.width() * SPRITE_SCALE_FACTOR, sprite_pixmap.height() * SPRITE_SCALE_FACTOR);

        QLabel* sprite = result_groupbox->findChild<QLabel*>("sprite");
        sprite->setPixmap(sprite_pixmap);

        result_groupbox->findChild<QLabel*>("hp_evs")->setText(tr("HP EVS: ")+QString::number(std::get<0>(theResult)));
        result_groupbox->findChild<QLabel*>("def_evs")->setText(tr("Defense EVS: ")+QString::number(std::get<1>(theResult)));
        result_groupbox->findChild<QLabel*>("spdef_evs")->setText(tr("Sp. Defense EVS: ")+QString::number(std::get<2>(theResult)));

        for( unsigned int it = 0; it < theTurns.size(); it++ ) {

            QString final_result = getCompleteString(theTurns[it], theDefendingPokemon, theResult, theDefModifier[it], true, theRoll[it], theDamagePerc[it]);
            calc_groupbox->findChild<QTextEdit*>("text_edit")->setText(calc_groupbox->findChild<QTextEdit*>("text_edit")->toPlainText() + final_result + "\n\n");
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
    QString hp_evs = QString::number(std::get<0>(theResult)) + " HP / ";

    //def
    QString def_evs;
    if(theMove.getMoveCategory() == Move::PHYSICAL || isDualDefense ) {
        if( std::get<1>(theDefModifier) != 0 ) {
            if( std::get<1>(theDefModifier) > 0 ) def_evs = def_evs + "+";
            def_evs = def_evs + QString::number(std::get<1>(theDefModifier)) + " ";
        }

        def_evs = def_evs + QString::number(std::get<1>(theResult));

        if( thePokemon.getNature() == Stats::BOLD || thePokemon.getNature() == Stats::RELAXED || thePokemon.getNature() == Stats::IMPISH || thePokemon.getNature() == Stats::LAX ) def_evs = def_evs + "+ Def / ";
        else if( thePokemon.getNature() == Stats::HASTY || thePokemon.getNature() == Stats::MILD || thePokemon.getNature() == Stats::LONELY || thePokemon.getNature() == Stats::GENTLE ) def_evs = def_evs + "- Def / ";
        else def_evs = def_evs + " Def / ";
    }

    //spdef
    QString spdef_evs;
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
    QString roll_result;
    if(theTurn.getHits() > 1) roll_result = QString::number(100-theRoll, 'f', 1) + "% chance of resisting " + QString::number(theTurn.getHits()) + " moves ";
    else roll_result = QString::number(100-theRoll, 'f', 1) + "% chance of resisting " + QString::number(theTurn.getHits()) + " move ";

    //DAMAGE
    QString damage_result = "(" + QString::number(*theDamagePerc.begin(), 'f', 1) + "% - " + QString::number(theDamagePerc.back(), 'f', 1) + "%)" /*+ "\n("*/;

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

QString ResultWindow::getCompleteString(const Turn& theTurn, const Pokemon& theDefendingPokemon, const std::tuple<int, int, int>& theResult, const defense_modifier& theDefModifier, const bool isDualDefense, const float theRoll, const std::vector<float>& theDamagePerc) {
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
    defender_result = vs_string + getDefendPokemon(theDefendingPokemon, theResult, theDefModifier, theTurn.getMoves()[0].second, true);

    //MODIFIERS
    modifier_result = getModifiers(theTurn);

    //RESULT
    damage_result = getResults(theTurn, theDefendingPokemon, theRoll, theDamagePerc);

    return first_result + second_result + defender_result + modifier_result + damage_result;
}
