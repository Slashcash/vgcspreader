#ifndef PRESETWINDOW_HPP
#define PRESETWINDOW_HPP

#include <QDialog>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class PresetWindow : public QDialog {
    Q_OBJECT

    private slots:
        void deletePreset(bool clicked);
        void savePreset(bool clicked);

    private:
        QGroupBox* group_box;

    public:
        QDialogButtonBox* bottom_buttons;

        PresetWindow(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

        void loadComboBox(const std::vector<QString>& theNames);
};

#endif // PRESETWINDOW_HPP
