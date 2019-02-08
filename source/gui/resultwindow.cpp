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

void ResultWindow::setResult(const Pokemon& theDefendingPokemon, const std::vector<Turn>& theTurns, const std::tuple<int, int, int>& theResult, const std::vector<float>& theRoll) {
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

        for( auto it = theTurns.begin(); it < theTurns.end(); it++ ) {
            QString final_result;

            QString first_result;
            QString second_result;
            QString defender_result;
            QString modifier_result;
            QString roll_result;

            //FIRST POKEMON

            //atk1 evs
            QString atk1_evs;
            if( it->getMoves()[0].second.getMoveCategory() == Move::Category::SPECIAL ) {
                if( it->getMoves()[0].first.getModifier(Stats::SPATK) != 0 ) {
                    if( it->getMoves()[0].first.getModifier(Stats::SPATK) > 0 ) atk1_evs = atk1_evs + "+";
                    atk1_evs = atk1_evs + QString::number(it->getMoves()[0].first.getModifier(Stats::SPATK)) + " ";
                }

                atk1_evs = atk1_evs + QString::number(it->getMoves()[0].first.getEV(Stats::SPATK));
                if( it->getMoves()[0].first.getNature() == Stats::MODEST || it->getMoves()[0].first.getNature() == Stats::QUIET|| it->getMoves()[0].first.getNature() == Stats::MILD || it->getMoves()[0].first.getNature() == Stats::RASH ) atk1_evs = atk1_evs + "+ SpA ";
                else if( it->getMoves()[0].first.getNature() == Stats::ADAMANT || it->getMoves()[0].first.getNature() == Stats::IMPISH || it->getMoves()[0].first.getNature() == Stats::JOLLY || it->getMoves()[0].first.getNature() == Stats::CAREFUL ) atk1_evs = atk1_evs + "- SpA ";
                else atk1_evs = atk1_evs + " SpA ";
            }

            else {
                if( it->getMoves()[0].first.getModifier(Stats::ATK) != 0 ) {
                    if( it->getMoves()[0].first.getModifier(Stats::ATK) > 0 ) atk1_evs = atk1_evs + "+";
                    atk1_evs = atk1_evs + QString::number(it->getMoves()[0].first.getModifier(Stats::ATK)) + " ";
                }

                atk1_evs = atk1_evs + QString::number(it->getMoves()[0].first.getEV(Stats::ATK));
                if( it->getMoves()[0].first.getNature() == Stats::LONELY || it->getMoves()[0].first.getNature() == Stats::BRAVE || it->getMoves()[0].first.getNature() == Stats::ADAMANT || it->getMoves()[0].first.getNature() == Stats::NAUGHTY ) atk1_evs = atk1_evs + "+ Atk ";
                else if( it->getMoves()[0].first.getNature() == Stats::BOLD || it->getMoves()[0].first.getNature() == Stats::TIMID || it->getMoves()[0].first.getNature() == Stats::MODEST || it->getMoves()[0].first.getNature() == Stats::CALM ) atk1_evs = atk1_evs + "- Atk ";
                else atk1_evs = atk1_evs + " Atk ";
            }

            //atk1_item
            QString atk1_item;
            if( it->getMoves()[0].first.getItem().getIndex() != Items::None ) atk1_item = atk1_item + ((MainWindow*)parentWidget())->getItemsNames()[it->getMoves()[0].first.getItem().getIndex()] + " ";

            //atk1_ability
            QString atk1_ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[it->getMoves()[0].first.getAbility()] + " ";

            //atk1_pokemon
            QString atk1_pokemon = ((MainWindow*)parentWidget())->getSpeciesNames()[it->getMoves()[0].first.getPokedexNumber()-1] + " ";

            //atk1_move
            QString atk1_move;
            if( it->getMoves()[0].second.isZ() ) atk1_move = atk1_move + "Z-";
            atk1_move = atk1_move + ((MainWindow*)parentWidget())->getMovesNames()[it->getMoves()[0].second.getMoveIndex()] + " ";

            first_result = atk1_evs + atk1_item + atk1_ability + atk1_pokemon + atk1_move;

            if( it->getMoveNum() > 1 ) {
                //SECOND POKEMON
                QString plus_sign = "+ ";

                //atk2_evs
                QString atk2_evs;
                if( it->getMoves()[1].second.getMoveCategory() == Move::Category::SPECIAL ) {
                    if( it->getMoves()[1].first.getModifier(Stats::SPATK) != 0 ) {
                        if( it->getMoves()[1].first.getModifier(Stats::SPATK) > 0 ) atk2_evs = atk2_evs + "+";
                        atk2_evs = atk2_evs + QString::number(it->getMoves()[1].first.getModifier(Stats::SPATK)) + " ";
                    }

                    atk2_evs = atk2_evs + QString::number(it->getMoves()[1].first.getEV(Stats::SPATK));
                    if( it->getMoves()[1].first.getNature() == Stats::MODEST || it->getMoves()[1].first.getNature() == Stats::QUIET|| it->getMoves()[1].first.getNature() == Stats::MILD || it->getMoves()[1].first.getNature() == Stats::RASH ) atk2_evs = atk2_evs + "+ SpA ";
                    else if( it->getMoves()[1].first.getNature() == Stats::ADAMANT || it->getMoves()[1].first.getNature() == Stats::IMPISH || it->getMoves()[1].first.getNature() == Stats::JOLLY || it->getMoves()[1].first.getNature() == Stats::CAREFUL ) atk2_evs = atk2_evs + "- SpA ";
                    else atk2_evs = atk2_evs + " SpA ";
                }

                else {
                    if( it->getMoves()[1].first.getModifier(Stats::ATK) != 0 ) {
                        if( it->getMoves()[1].first.getModifier(Stats::ATK) > 0 ) atk2_evs = atk2_evs + "+";
                        atk2_evs = atk2_evs + QString::number(it->getMoves()[1].first.getModifier(Stats::ATK)) + " ";
                    }

                    atk2_evs = atk2_evs + QString::number(it->getMoves()[1].first.getEV(Stats::ATK));
                    if( it->getMoves()[1].first.getNature() == Stats::LONELY || it->getMoves()[1].first.getNature() == Stats::BRAVE || it->getMoves()[1].first.getNature() == Stats::ADAMANT || it->getMoves()[1].first.getNature() == Stats::NAUGHTY ) atk2_evs = atk2_evs + "+ Atk ";
                    else if( it->getMoves()[1].first.getNature() == Stats::BOLD || it->getMoves()[1].first.getNature() == Stats::TIMID || it->getMoves()[1].first.getNature() == Stats::MODEST || it->getMoves()[1].first.getNature() == Stats::CALM ) atk2_evs = atk2_evs + "- Atk ";
                    else atk2_evs = atk2_evs + " Atk ";
                }

                //atk2_item
                QString atk2_item;
                if( it->getMoves()[1].first.getItem().getIndex() != Items::None ) atk2_item = atk2_item + ((MainWindow*)parentWidget())->getItemsNames()[it->getMoves()[1].first.getItem().getIndex()] + " ";

                //atk2_ability
                QString atk2_ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[it->getMoves()[1].first.getAbility()] + " ";

                //atk2_pokemon
                QString atk2_pokemon = ((MainWindow*)parentWidget())->getSpeciesNames()[it->getMoves()[1].first.getPokedexNumber()-1] + " ";

                //atk2_move
                QString atk2_move;
                if( it->getMoves()[1].second.isZ() ) atk2_move = atk2_move + "Z-";
                atk2_move = atk2_move + ((MainWindow*)parentWidget())->getMovesNames()[it->getMoves()[1].second.getMoveIndex()] + " ";

                second_result = plus_sign + atk2_evs + atk2_item + atk2_ability + atk2_pokemon + atk2_move;
            }

            //DEFENDING POKEMON
            QString vs_string = "vs. ";

            //hp
            QString def_hp_evs = QString::number(std::get<0>(theResult)) + " HP / ";

            //def
            QString def_def_evs = QString::number(std::get<1>(theResult)) + " Def / ";

            //spdef
            QString def_spdef_evs = QString::number(std::get<2>(theResult)) + " SpD ";

            //item
            QString def_item;
            if( theDefendingPokemon.getItem().getIndex() != Items::None ) def_item = ((MainWindow*)parentWidget())->getItemsNames()[theDefendingPokemon.getItem().getIndex()] + " ";

            //ability
            QString def_ability = ((MainWindow*)parentWidget())->getAbilitiesNames()[theDefendingPokemon.getAbility()] + " ";

            //species
            QString def_species = ((MainWindow*)parentWidget())->getSpeciesNames()[theDefendingPokemon.getPokedexNumber()-1] + " ";

            defender_result = vs_string + def_hp_evs + def_def_evs + def_spdef_evs + def_item + def_ability + def_species;

            //MODIFIERS

            //RESULT
            roll_result = roll_result + "-- ";
            roll_result = roll_result + QString::number(100-theRoll[std::distance(theTurns.begin(), it)], 'f', 1) + "% chanche of resisting " + QString::number(it->getHits()) + " moves";

            final_result = first_result + second_result + defender_result + modifier_result + roll_result + "\n\n";

            text_edit->setText(text_edit->toPlainText() + final_result);
        }
    }
}

