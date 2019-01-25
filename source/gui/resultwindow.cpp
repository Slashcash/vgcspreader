#include "resultwindow.hpp"

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

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void ResultWindow::setResult(const Pokemon& theDefendingPokemon, const std::vector<Turn>& theTurns, const std::tuple<int, int, int>& theResult, const std::vector<float>& theRoll) {
    if( std::get<0>(theResult) == -1 || std::get<1>(theResult) == -1 || std::get<2>(theResult) == -1  ) {
        def_evs->setVisible(false);
        spdef_evs->setVisible(false);

        hp_evs->setText(tr("Sorry, no such spread exists"));
    }

    else {
        hp_evs->setVisible(true);
        def_evs->setVisible(true);
        spdef_evs->setVisible(true);

        hp_evs->setText(tr("HP EVS: ")+QString::number(std::get<0>(theResult)));
        def_evs->setText(tr("Defense EVS: ")+QString::number(std::get<1>(theResult)));
        spdef_evs->setText(tr("Sp. Defense EVS: ")+QString::number(std::get<2>(theResult)));
    }
}

