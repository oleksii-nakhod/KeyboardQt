#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QAbstractButton>
#include <QFile>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    QString frequency_order = "eariotnslcudpmhgbfywkvxzjq";
private slots:
    void group_button_clicked(QAbstractButton*);

    void on_button_auto_clicked();

    void on_button_manual_clicked();

    void on_button_strict_clicked();

    void on_button_simple_clicked();

    void on_slider_wordsamount_valueChanged(int value);

    void on_checkbox_focus_clicked();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
