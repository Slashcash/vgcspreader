#include "resultwindow.hpp"
#include "mainwindow.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>

ResultWindow::ResultWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    QHBoxLayout* main_layout = new QHBoxLayout;
    QVBoxLayout* result_layout = new QVBoxLayout;

    //result groupbox
    result_groupbox = new QGroupBox("Result:");
    main_layout->addWidget(result_groupbox);
    result_groupbox->setLayout(result_layout);

    hp_evs = new QLabel(tr("HP EVS: ")+"0");
    result_layout->addWidget(hp_evs);

    def_evs = new QLabel(tr("Defense EVS: ")+"0");
    result_layout->addWidget(def_evs);

    spdef_evs = new QLabel(tr("Sp. Defense EVS: ")+"0");
    result_layout->addWidget(spdef_evs);

    setLayout(main_layout);

    text_edit = new QTextEdit;
    main_layout->addWidget(text_edit);

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void ResultWindow::setResult(const Pokemon& theDefendingPokemon, const std::vector<defense_modifier>& theDefModifier, const std::vector<Turn>& theTurns, const std::tuple<int, int, int>& theResult, const std::vector<std::vector<float>>& theDamagePerc, const std::vector<std::vector<int>>& theDamageInt, const std::vector<float>& theRoll) {
    if( std::get<0>(theResult) == -1 || std::get<1>(theResult) == -1 || std::get<2>(theResult) == -1  ) {
        def_evs->setVisible(false);
        spdef_evs->setVisible(false);

        hp_evs->setText(tr("Sorry, no such spread exists"));
        text_edit->setVisible(false);
    }

    else {
        text_edit->setText("");
        hp_evs->setVisible(true);
        def_evs->setVisible(true);
        spdef_evs->setVisible(true);
        text_edit->setVisible(true);

        hp_evs->setText(tr("HP EVS: ")+QString::number(std::get<0>(theResult)));
        def_evs->setText(tr("Defense EVS: ")+QString::number(std::get<1>(theResult)));
        spdef_evs->setText(tr("Sp. Defense EVS: ")+QString::number(std::get<2>(theResult)));

        for( auto it = 0; it < theTurns.size(); it++ ) {
            QString final_result;

            QString first_result;
            QString second_result;
            QString defender_result;
            QString modifier_result;
            QString roll_result;
            QString damage_result;

            //FIRST POKEMON

            //atk1 evs
            QString atk1_evs;
            if( theTurns[it].getMoves()[0].second.getMoveCategory() == Move::Category::SPECIAL ) {
                if( theTurns[it].getMoves()[0].first.getModifier(Stats::SPATK) != 0 ) {
                    if( theTurns[it].getMoves()[0].first.getModifier(Stats::SPATK) > 0 ) atk1_evs = atk1_evs + "+";
                    atk1_evs = atk1_evs + QString::number(theTurns[it].getMoves()[0].first.getModifier(Stats::SPATK)) + " ";
                }

                atk1_evs = atk1_evs + QString::number(theTurns[it].getMoves()[0].first.getEV(Stats::SPATK));
                if( theTurns[it].getMoves()[0].first.getNature() == Stats::MODEST || theTurns[it].getMoves()[0].first.getNature() == Stats::QUIET|| theTurns[it].getMoves()[0].first.getNature() == Stats::MILD || theTurns[it].getMoves()[0].first.getNature() == Stats::RASH ) atk1_evs = atk1_evs + "+ SpA ";
                else if( theTurns[it].getMoves()[0].first.getNature() == Stats::ADAMANT || theTurns[it].getMoves()[0].first.getNature() == Stats::IMPISH || theTurns[it].getMoves()[0].first.getNature() == Stats::JOLLY || theTurns[it].getMoves()[0].first.getNature() == Stats::CAREFUL ) atk1_evs = atk1_evs + "- SpA ";
                else atk1_evs = atk1_evs + " SpA ";
            }

            else {
                if( theTurns[it].getMoves()[0].first.getModifier(Stats::ATK) != 0 ) {
                    if( theTurns[it].getMoves()[0].first.getModifier(Stats::ATK) > 0 ) atk1_evs = atk1_evs + "+";
                    atk1_evs = atk1_evs + QString::number(theTurns[it].getMoves()[0].first.getModifier(Stats::ATK)) + " ";
                }

                atk1_evs = atk1_evs + QString::number(theTurns[it].getMoves()[0].first.getEV(Stats::ATK));
                if( theTurns[it].getMoves()[0].first.getNature() == Stats::LONELY || theTurns[it].getMoves()[0].first.getNature() == Stats::BRAVE || theTurns[it].getMoves()[0].first.getNature() == Stats::ADAMANT || theTurns[it].getMoves()[0].first.getNature() == Stats::NAUGHTY ) atk1_evs = atk1_evs + "+ Atk ";
                else if( theTurns[it].getMoves()[0].first.getNature() == Stats::BOLD || theTurns[it].getMoves()[0].first.getNature() == Stats::TIMID || theTurns[it].getMoves()[0].first.getNature() == Stats::MODEST || theTurns[it].getMoves()[0].first.getNature() == Stats::CALM ) atk1_evs = atk1_evs + "- Atk ";
                else atk1_evs = atk1_evs + " Atk ";
            }

            //atk1_item
            QString atk1_item;
            if( theTurns[it].getMoves()[0].first.getItem().getIndex() != Items::None ) atk1_item = atk1_item + ((MainWindow*)parentWidget())->getItemsNames()[theTurns[it].getMoves()[0].first.getItem().getIndex()] + " ";

            //atk1_ability
            QString atk1_ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[theTurns[it].getMoves()[0].first.getAbility()] + " ";

            //atk1_pokemon
            QString atk1_pokemon = ((MainWindow*)parentWidget())->getSpeciesNames()[theTurns[it].getMoves()[0].first.getPokedexNumber()-1] + " ";

            //atk1_move
            QString atk1_move;
            if( theTurns[it].getMoves()[0].second.isZ() ) atk1_move = atk1_move + "Z-";
            atk1_move = atk1_move + ((MainWindow*)parentWidget())->getMovesNames()[theTurns[it].getMoves()[0].second.getMoveIndex()] + " ";

            //atk1 crit
            QString atk1_crit;
            if( theTurns[it].getMoves()[0].second.isCrit() ) atk1_crit = atk1_crit + "on a critical hit ";

            first_result = atk1_evs + atk1_item + atk1_ability + atk1_pokemon + atk1_move + atk1_crit;

            if( theTurns[it].getMoveNum() > 1 ) {
                //SECOND POKEMON
                QString plus_sign = "[+] ";

                //atk2_evs
                QString atk2_evs;
                if( theTurns[it].getMoves()[1].second.getMoveCategory() == Move::Category::SPECIAL ) {
                    if( theTurns[it].getMoves()[1].first.getModifier(Stats::SPATK) != 0 ) {
                        if( theTurns[it].getMoves()[1].first.getModifier(Stats::SPATK) > 0 ) atk2_evs = atk2_evs + "+";
                        atk2_evs = atk2_evs + QString::number(theTurns[it].getMoves()[1].first.getModifier(Stats::SPATK)) + " ";
                    }

                    atk2_evs = atk2_evs + QString::number(theTurns[it].getMoves()[1].first.getEV(Stats::SPATK));
                    if( theTurns[it].getMoves()[1].first.getNature() == Stats::MODEST || theTurns[it].getMoves()[1].first.getNature() == Stats::QUIET|| theTurns[it].getMoves()[1].first.getNature() == Stats::MILD || theTurns[it].getMoves()[1].first.getNature() == Stats::RASH ) atk2_evs = atk2_evs + "+ SpA ";
                    else if( theTurns[it].getMoves()[1].first.getNature() == Stats::ADAMANT || theTurns[it].getMoves()[1].first.getNature() == Stats::IMPISH || theTurns[it].getMoves()[1].first.getNature() == Stats::JOLLY || theTurns[it].getMoves()[1].first.getNature() == Stats::CAREFUL ) atk2_evs = atk2_evs + "- SpA ";
                    else atk2_evs = atk2_evs + " SpA ";
                }

                else {
                    if( theTurns[it].getMoves()[1].first.getModifier(Stats::ATK) != 0 ) {
                        if( theTurns[it].getMoves()[1].first.getModifier(Stats::ATK) > 0 ) atk2_evs = atk2_evs + "+";
                        atk2_evs = atk2_evs + QString::number(theTurns[it].getMoves()[1].first.getModifier(Stats::ATK)) + " ";
                    }

                    atk2_evs = atk2_evs + QString::number(theTurns[it].getMoves()[1].first.getEV(Stats::ATK));
                    if( theTurns[it].getMoves()[1].first.getNature() == Stats::LONELY || theTurns[it].getMoves()[1].first.getNature() == Stats::BRAVE || theTurns[it].getMoves()[1].first.getNature() == Stats::ADAMANT || theTurns[it].getMoves()[1].first.getNature() == Stats::NAUGHTY ) atk2_evs = atk2_evs + "+ Atk ";
                    else if( theTurns[it].getMoves()[1].first.getNature() == Stats::BOLD || theTurns[it].getMoves()[1].first.getNature() == Stats::TIMID || theTurns[it].getMoves()[1].first.getNature() == Stats::MODEST || theTurns[it].getMoves()[1].first.getNature() == Stats::CALM ) atk2_evs = atk2_evs + "- Atk ";
                    else atk2_evs = atk2_evs + " Atk ";
                }

                //atk2_item
                QString atk2_item;
                if( theTurns[it].getMoves()[1].first.getItem().getIndex() != Items::None ) atk2_item = atk2_item + ((MainWindow*)parentWidget())->getItemsNames()[theTurns[it].getMoves()[1].first.getItem().getIndex()] + " ";

                //atk2_ability
                QString atk2_ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[theTurns[it].getMoves()[1].first.getAbility()] + " ";

                //atk2_pokemon
                QString atk2_pokemon = ((MainWindow*)parentWidget())->getSpeciesNames()[theTurns[it].getMoves()[1].first.getPokedexNumber()-1] + " ";

                //atk2_move
                QString atk2_move;
                if( theTurns[it].getMoves()[1].second.isZ() ) atk2_move = atk2_move + "Z-";
                atk2_move = atk2_move + ((MainWindow*)parentWidget())->getMovesNames()[theTurns[it].getMoves()[1].second.getMoveIndex()] + " ";

                //atk2 crit
                QString atk2_crit;
                if( theTurns[it].getMoves()[1].second.isCrit() ) atk2_crit = atk2_crit + "on a critical hit ";

                second_result = plus_sign + atk2_evs + atk2_item + atk2_ability + atk2_pokemon + atk2_move + atk2_crit;
            }

            //DEFENDING POKEMON
            QString vs_string = "vs. ";

            //hp
            QString def_hp_evs = QString::number(std::get<0>(theResult)) + " HP / ";

            //def
            QString def_def_evs;
            if( std::get<1>(theDefModifier[it]) != 0 ) {
                if( std::get<1>(theDefModifier[it]) > 0 ) def_def_evs = def_def_evs + "+";
                def_def_evs = def_def_evs + QString::number(std::get<1>(theDefModifier[it])) + " ";
            }

            def_def_evs = def_def_evs + QString::number(std::get<1>(theResult));

            if( theDefendingPokemon.getNature() == Stats::BOLD || theDefendingPokemon.getNature() == Stats::RELAXED || theDefendingPokemon.getNature() == Stats::IMPISH || theDefendingPokemon.getNature() == Stats::LAX ) def_def_evs = def_def_evs + "+ Def / ";
            else if( theDefendingPokemon.getNature() == Stats::HASTY || theDefendingPokemon.getNature() == Stats::MILD || theDefendingPokemon.getNature() == Stats::LONELY || theDefendingPokemon.getNature() == Stats::GENTLE ) def_def_evs = def_def_evs + "- Def / ";
            else def_def_evs = def_def_evs + " Def / ";

            //spdef
            QString def_spdef_evs;
            if( std::get<2>(theDefModifier[it]) != 0 ) {
                if( std::get<2>(theDefModifier[it]) > 0 ) def_spdef_evs = def_spdef_evs + "+";
                def_spdef_evs = def_spdef_evs + QString::number(std::get<2>(theDefModifier[it])) + " ";
            }

            def_spdef_evs = def_spdef_evs + QString::number(std::get<2>(theResult));

            if( theDefendingPokemon.getNature() == Stats::CALM || theDefendingPokemon.getNature() == Stats::GENTLE || theDefendingPokemon.getNature() == Stats::SASSY || theDefendingPokemon.getNature() == Stats::CAREFUL ) def_spdef_evs = def_spdef_evs + "+ SpD ";
            else if( theDefendingPokemon.getNature() == Stats::NAUGHTY || theDefendingPokemon.getNature() == Stats::LAX || theDefendingPokemon.getNature() == Stats::NAIVE || theDefendingPokemon.getNature() == Stats::RASH ) def_spdef_evs = def_spdef_evs + "- SpD ";
            else def_spdef_evs = def_spdef_evs + " SpD ";

            //item
            QString def_item;
            if( theDefendingPokemon.getItem().getIndex() != Items::None ) def_item = ((MainWindow*)parentWidget())->getItemsNames()[theDefendingPokemon.getItem().getIndex()] + " ";

            //ability
            QString def_ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[theDefendingPokemon.getAbility()] + " ";

            //species
            QString def_species = ((MainWindow*)parentWidget())->getSpeciesNames()[theDefendingPokemon.getPokedexNumber()-1] + " ";

            defender_result = vs_string + def_hp_evs + def_def_evs + def_spdef_evs + def_item + def_ability + def_species;

            //MODIFIERS
            //rain
            bool in_rain = false;
            bool rain_affected_move = false;
            for( auto it2 = theTurns[it].getMoves().begin(); it2 < theTurns[it].getMoves().end(); it2++ ) {
                if( it2->second.getWeather() == Move::Weather::RAIN ) in_rain = true;
                if( it2->second.getMoveType() == Type::Fire || it2->second.getMoveType() == Type::Water ) rain_affected_move = true;
            }

            if( in_rain && rain_affected_move ) modifier_result = modifier_result + "in Rain ";

            //sun
            bool in_sun = false;
            bool sun_affected_move = false;
            for( auto it2 = theTurns[it].getMoves().begin(); it2 < theTurns[it].getMoves().end(); it2++ ) {
                if( it2->second.getWeather() == Move::Weather::SUN ) in_sun = true;
                if( it2->second.getMoveType() == Type::Fire || it2->second.getMoveType() == Type::Water ) sun_affected_move = true;
            }

            if( in_sun && sun_affected_move ) modifier_result = modifier_result + "in Sun ";

            //electric
            bool in_electric = false;
            bool electric_affected_move = false;
            for( auto it2 = theTurns[it].getMoves().begin(); it2 < theTurns[it].getMoves().end(); it2++ ) {
                if( it2->second.getTerrain() == Move::Terrain::ELECTRIC ) in_electric = true;
                if( it2->second.getMoveType() == Type::Electric && it2->first.isGrounded() ) electric_affected_move = true;
            }

            if( in_electric && electric_affected_move ) modifier_result = modifier_result + "in Electric Terrain ";

            //psychic
            bool in_psychic = false;
            bool psychic_affected_move = false;
            for( auto it2 = theTurns[it].getMoves().begin(); it2 < theTurns[it].getMoves().end(); it2++ ) {
                if( it2->second.getTerrain() == Move::Terrain::PSYCHIC ) in_psychic = true;
                if( it2->second.getMoveType() == Type::Psychic && it2->first.isGrounded() ) psychic_affected_move = true;
            }

            if( in_psychic && psychic_affected_move ) modifier_result = modifier_result + "in Psychic Terrain ";

            //grassy
            bool in_grassy = false;
            bool grassy_affected_move = false;
            for( auto it2 = theTurns[it].getMoves().begin(); it2 < theTurns[it].getMoves().end(); it2++ ) {
                if( it2->second.getTerrain() == Move::Terrain::GRASSY ) in_grassy = true;
                if( it2->second.getMoveType() == Type::Grass && it2->first.isGrounded() ) grassy_affected_move = true;
            }

            if( in_grassy && grassy_affected_move ) modifier_result = modifier_result + "in Grassy Terrain ";

            //misty
            bool in_misty = false;
            bool misty_affected_move = false;
            for( auto it2 = theTurns[it].getMoves().begin(); it2 < theTurns[it].getMoves().end(); it2++ ) {
                if( it2->second.getTerrain() == Move::Terrain::MISTY ) in_misty = true;
                if( it2->second.getMoveType() == Type::Dragon && it2->first.isGrounded() ) misty_affected_move = true;
            }

            if( in_misty && misty_affected_move ) modifier_result = modifier_result + "in Misty Terrain ";


            //RESULT
            roll_result = roll_result + "-- ";
            roll_result = roll_result + QString::number(100-theRoll[it], 'f', 1) + "% chance of resisting " + QString::number(theTurns[it].getHits()) + " moves ";

            //DAMAGE
            damage_result = damage_result + "(" + QString::number(*theDamagePerc[it].begin(), 'f', 1) + "% - " + QString::number(theDamagePerc[it].back(), 'f', 1) + "%)\n(";
            for(auto it_temp = theDamageInt[it].begin(); it_temp < theDamageInt[it].end()-1; it_temp++) damage_result = damage_result + QString::number(*it_temp) + ", ";
            damage_result = damage_result + QString::number(theDamageInt[it].back()) + ") ";

            //RESTORER
            //grassy terrain restore
            QString restore_result;
            bool grassy_terrain_recover = false;
            auto turn_moves = theTurns[it].getMoves();
            for( auto it_turn = turn_moves.begin(); it_turn < turn_moves.end(); it_turn++ )
                if( it_turn->second.getTerrain() == Move::Terrain::GRASSY && theDefendingPokemon.isGrounded() ) grassy_terrain_recover = true;

            if( grassy_terrain_recover ) restore_result = restore_result + "after Grassy Terrain recover ";

            final_result = first_result + second_result + defender_result + modifier_result + roll_result + damage_result + restore_result + "\n\n";

            text_edit->setText(text_edit->toPlainText() + final_result);
        }
    }
}

