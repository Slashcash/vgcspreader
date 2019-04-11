#ifndef ALERTWINDOW_HPP
#define ALERTWINDOW_HPP

#include <QDialog>
#include <QDialogButtonBox>

class AlertWindow : public QDialog {
    Q_OBJECT

    public:
        QDialogButtonBox* bottom_buttons;

        AlertWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
};

#endif // ALERTWINDOW_HPP
