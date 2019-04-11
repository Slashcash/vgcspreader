#include "alertwindow.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

AlertWindow::AlertWindow(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    setObjectName("AlertWindow");
    setWindowTitle("VGCSpreader");

    QVBoxLayout* layout = new QVBoxLayout;

    QLabel* msg = new QLabel(tr("This calculation may take a very (very) long time. Would you like to continue?\nPlease note that it can be aborted at any time by clicking the STOP button"));
    layout->addWidget(msg);

    bottom_buttons = new QDialogButtonBox;

    QPushButton* yes_button = new QPushButton(tr("Yes"));
    QPushButton* no_button = new QPushButton(tr("No"));

    bottom_buttons->addButton(yes_button, QDialogButtonBox::ButtonRole::AcceptRole);
    bottom_buttons->addButton(no_button, QDialogButtonBox::ButtonRole::RejectRole);

    connect(bottom_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bottom_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(bottom_buttons);

    setLayout(layout);
    layout->setSizeConstraint( QLayout::SetFixedSize );
}
