#ifndef ADDPRESETWINDOW_HPP
#define ADDPRESETWINDOW_HPP

#include <QDialog>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class AddPresetWindow : public QDialog {
    Q_OBJECT

    private slots:
        void savePreset(bool clicked);

    private:
        QGroupBox* group_box;

    public:
        QDialogButtonBox* bottom_buttons;

        AddPresetWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
};

#endif // ADDPRESETWINDOW_HPP
