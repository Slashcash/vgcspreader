#include "presetwindow.hpp"

#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>

#include "mainwindow.hpp"

PresetWindow::PresetWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    setObjectName("PresetWindow");
    setWindowTitle("VGCSpreader");

    QVBoxLayout* main_layout = new QVBoxLayout;
    setLayout(main_layout);

    QVBoxLayout* layout = new QVBoxLayout;
    group_box = new QGroupBox;
    group_box->setLayout(layout);

    QComboBox* preset_combobox = new QComboBox;
    preset_combobox->setObjectName("preset_combobox");
    layout->addWidget(preset_combobox);

    QLabel* no_presets_label = new QLabel(tr("No presets yet. Please add a preset first!"));
    no_presets_label->setAlignment(Qt::AlignCenter);
    no_presets_label->setObjectName("no_presets_label");
    layout->addWidget(no_presets_label);

    bottom_buttons = new QDialogButtonBox;
    bottom_buttons->setObjectName("bottom_buttons");

    QPushButton* yes_button = new QPushButton(tr("Add"));
    QPushButton* no_button = new QPushButton(tr("Cancel"));
    QPushButton* delete_button = new QPushButton(tr("Delete Preset"));

    bottom_buttons->addButton(delete_button, QDialogButtonBox::ButtonRole::DestructiveRole);
    bottom_buttons->addButton(yes_button, QDialogButtonBox::ButtonRole::AcceptRole);
    bottom_buttons->addButton(no_button, QDialogButtonBox::ButtonRole::RejectRole);

    connect(bottom_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bottom_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(delete_button, SIGNAL(clicked(bool)), this, SLOT(deletePreset(bool)));
    connect(yes_button, SIGNAL(clicked(bool)), this, SLOT(savePreset(bool)));


    layout->addWidget(bottom_buttons);

    main_layout->addWidget(group_box);
}

void PresetWindow::deletePreset(bool clicked) {
    QComboBox* temp = group_box->findChild<QComboBox*>("preset_combobox");

    auto delete_node = ((MainWindow*)parentWidget())->xml_preset.RootElement()->FirstChildElement();
    while( delete_node->GetText() == temp->currentText().toStdString().c_str() ) delete_node = delete_node->NextSiblingElement();

    ((MainWindow*)parentWidget())->xml_preset.RootElement()->DeleteChild(delete_node);
    ((MainWindow*)parentWidget())->xml_preset.SaveFile("presets.xml");

    ((MainWindow*)parentWidget())->presets.erase(((MainWindow*)parentWidget())->presets.begin() + temp->currentIndex());
    temp->removeItem(temp->currentIndex());

    if( temp->count() == 0 ) close();
}

void PresetWindow::loadComboBox(const std::vector<QString>& theNames) {
    QComboBox* temp = group_box->findChild<QComboBox*>("preset_combobox");
    temp->clear();
    for(auto it = theNames.begin(); it < theNames.end(); it++) temp->addItem(*it);

    if( temp->count() == 0 ) {
        temp->setVisible(false);
        group_box->findChild<QLabel*>("no_presets_label")->setVisible(true);
        group_box->findChild<QDialogButtonBox*>("bottom_buttons")->setVisible(false);
    }

    else {
        temp->setVisible(true);
        group_box->findChild<QLabel*>("no_presets_label")->setVisible(false);
        group_box->findChild<QDialogButtonBox*>("bottom_buttons")->setVisible(true);
    }
}

void PresetWindow::savePreset(bool clicked) {
    ((MainWindow*)parentWidget())->solveMovePreset(group_box->findChild<QComboBox*>("preset_combobox")->currentIndex());
}
