#ifndef DEFENSEMOVEWINDOW_HPP
#define DEFENSEMOVEWINDOW_HPP

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
#include "addpresetwindow.hpp"

class DefenseMoveWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setSpecies1(int index);
        void setForm1(int index);
        void setForm2(int index);
        void setMove1(int index);
        void setMove2(int index);
        void setMoveCategory1(int index);
        void setMoveCategory2(int index);
        void setSpecies2(int index);
        void activateAtk2(int state);
        void addPreset(bool);

    public slots:
        void solveMove(const bool preset = false, const QString& preset_name = ""); //LAZYYYYYYYYY

    private:
        QGroupBox* atk1_groupbox;
        QGroupBox* atk2_groupbox;
        QGroupBox* defending_groupbox;
        QGroupBox* modifier_groupbox;
        QTabWidget* tabs;
        AddPresetWindow* addpreset_window;


        bool edit_mode;

        void createAtk1GroupBox();
        void createAtk2GroupBox();
        void createDefendingGroupBox();
        QString retrieveFormName(const int species, const int form);

    public:
        DefenseMoveWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        QDialogButtonBox* bottom_button_box; //HERE BECAUSE IT HAS TO BE ACCESSED BY MAINWINDOW

        void setAsBlank();
        void setAsTurn(const Turn& theTurn, const defense_modifier& theDefenseModifier);
        bool isEditMode() { return edit_mode; }

        void setEditMode(const bool theValue) { edit_mode = theValue; }
};

#endif // MOVEWINDOW_HPP
