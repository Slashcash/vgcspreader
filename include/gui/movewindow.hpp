#ifndef MOVEWINDOW_HPP
#define MOVEWINDOW_HPP

#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QTabWidget>

#include "turn.hpp"

class MoveWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setMove1(int index);
        void setSpecies1(int index);
        void setForm1(int index);
        void setForm2(int index);
        void setMove2(int index);
        void setSpecies2(int index);
        void activateAtk2(int state);
        void solveMove(void);

    private:
        QGroupBox* atk1_groupbox;
        QGroupBox* atk2_groupbox;
        QGroupBox* defending_groupbox;
        QGroupBox* modifier_groupbox;
        QTabWidget* tabs;

        bool edit_mode;

        void createAtk1GroupBox();
        void createAtk2GroupBox();
        void createDefendingGroupBox();
        QString retrieveFormName(const int species, const int form);

    public:
        MoveWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        QDialogButtonBox* bottom_button_box; //HERE BECAUSE IT HAS TO BE ACCESSED BY MAINWINDOW

        void setAsBlank();
        void setAsTurn(const Turn& theTurn, const defense_modifier& theDefenseModifier);
        bool isEditMode() { return edit_mode; }

        void setEditMode(const bool theValue) { edit_mode = theValue; }
};

#endif // MOVEWINDOW_HPP
