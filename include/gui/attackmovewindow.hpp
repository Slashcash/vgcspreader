#ifndef ATTACKMOVEWINDOW_HPP
#define ATTACKMOVEWINDOW_HPP

#include <QDialog>
#include <QGroupBox>
#include <QDialogButtonBox>

#include "turn.hpp"

class AttackMoveWindow : public QDialog {
    Q_OBJECT

    private slots:
        void setMove(int index);
        void setSpecies(int index);
        void setForm(int index);
        void solveMove(void);

    private:
        QGroupBox* defending_pokemon_groupbox;
        QGroupBox* move_groupbox;
        QGroupBox* move_modifier_groupbox;
        QGroupBox* atk_modifier_groupbox;

        bool edit_mode;

        void createDefendingPokemonGroupbox();
        void createMoveGroupbox();
        void createModifierGroupbox();

    public:
        QDialogButtonBox* bottom_button_box;

        AttackMoveWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        void setAsBlank();
        void setAsTurn(const Turn& theTurn, const Pokemon& theDefendingPokemon, const attack_modifier& theAttackModifier);
        void setEditMode(const bool isEditMode) { edit_mode = isEditMode; }
        bool isEditMode() const { return edit_mode; }

};

#endif // ATTACKMOVEWINDOW_HPP
