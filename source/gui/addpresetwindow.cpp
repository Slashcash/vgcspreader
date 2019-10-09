#include "addpresetwindow.hpp"

#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>

#include "defensemovewindow.hpp"

AddPresetWindow::AddPresetWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    setObjectName("AddPresetWindow");
    setWindowTitle("VGCSpreader");

    QVBoxLayout* main_layout = new QVBoxLayout; //the shittiest thing i've ever done
    setLayout(main_layout);

    QVBoxLayout* layout = new QVBoxLayout;
    group_box = new QGroupBox;
    group_box->setLayout(layout);

    QLineEdit* preset_name_textbox = new QLineEdit;
    preset_name_textbox->setObjectName("preset_name_textbox");

    QFormLayout* form_layout = new QFormLayout;
    form_layout->addRow(tr("Preset Name:"), preset_name_textbox);

    layout->addLayout(form_layout);

    bottom_buttons = new QDialogButtonBox;

    QPushButton* yes_button = new QPushButton(tr("Save"));
    QPushButton* no_button = new QPushButton(tr("Cancel"));

    bottom_buttons->addButton(yes_button, QDialogButtonBox::ButtonRole::AcceptRole);
    bottom_buttons->addButton(no_button, QDialogButtonBox::ButtonRole::RejectRole);

    connect(yes_button, SIGNAL(clicked(bool)), this, SLOT(savePreset(bool)));
    connect(bottom_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bottom_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(bottom_buttons);

    main_layout->addWidget(group_box);
}

void AddPresetWindow::savePreset(bool clicked) {
    ((DefenseMoveWindow*)parentWidget())->solveMove(true, group_box->findChild<QLineEdit*>("preset_name_textbox")->text());
    group_box->findChild<QLineEdit*>("preset_name_textbox")->clear();
}
