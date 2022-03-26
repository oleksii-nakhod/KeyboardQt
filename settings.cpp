#include "settings.h"
#include "ui_settings.h"
#include <QFile>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->buttonGroup->setExclusive(false);

    int words_amount = 5;
    QFile file("settings.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString dataText = file.readAll();
    if (dataText.contains("letters_mode_=auto"))
    {
        ui->button_auto->setChecked(true);
        foreach(QAbstractButton *button, ui->buttonGroup->buttons())
        {
            button->setDisabled(true);
        }
    }
    else if (dataText.contains("letters_mode_=manual"))
    {
        ui->button_manual->setChecked(true);
    }
    if (dataText.contains("typing_mode_=strict"))
    {
        ui->button_strict->setChecked(true);
    }
    else if (dataText.contains("typing_mode_=simple"))
    {
        ui->button_simple->setChecked(true);
    }
    if (dataText.contains("focus_mode_=1"))
    {
        ui->checkbox_focus->setChecked(true);

    }
    else if (dataText.contains("focus_mode_=0"))
    {
        ui->checkbox_focus->setChecked(false);

    }
    QTextStream file_in(&file);
    file.seek(0);
    while (!file_in.atEnd())
    {
        QString line = file_in.readLine();
        if (line.contains("words_amount_"))
        {
            int index = line.indexOf('=');
            words_amount = line.mid(index+1).toInt();
        }
    }
    foreach(QAbstractButton *button, ui->buttonGroup->buttons())
    {
        QString letter = button->objectName().right(1);
        QString letter_enabled = letter + "=1";
        QString letter_disabled = letter + "=0";
        if (dataText.contains(letter_enabled))
        {
            button->setStyleSheet("background-color: rgba(0, 255, 0, 0.4)");
        }
        else if (dataText.contains(letter_disabled))
        {
            button->setStyleSheet("background-color: rgba(255, 0, 0, 0.4)");
        }
    }
    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(group_button_clicked(QAbstractButton*)));
    ui->slider_wordsamount->setMinimum(5);
    ui->slider_wordsamount->setMaximum(25);
    ui->slider_wordsamount->setSingleStep(1);
    ui->slider_wordsamount->setValue(words_amount);
    ui->label_wordsamount->setText("Words Amount: " + QString::number(words_amount));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::group_button_clicked(QAbstractButton* button)
{
    QString letter_pressed = button->objectName().right(1);
    QString letter_enabled = letter_pressed + "=1";
    QString letter_disabled = letter_pressed + "=0";
    QFile file("settings.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QString dataText = file.readAll();
    if (dataText.contains(letter_enabled))
    {
        dataText.replace(letter_enabled, letter_disabled);
        button->setStyleSheet("background-color: rgba(255, 0, 0, 0.4)");
    }
    else if (dataText.contains(letter_disabled))
    {
        dataText.replace(letter_disabled, letter_enabled);
        button->setStyleSheet("background-color: rgba(0, 255, 0, 0.4)");
    }
    file.seek(0);
    QTextStream out(&file);
    out << dataText;
}

void Settings::on_button_auto_clicked()
{
    QString letter;
    int index;
    float stats;
    QFile file_settings("settings.txt");
    file_settings.open(QIODevice::ReadWrite | QIODevice::Text);
    QString dataText = file_settings.readAll();
    dataText.replace("letters_mode_=manual", "letters_mode_=auto");
    QFile file_temp("temp.txt");
    file_temp.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_temp(&file_temp);

    foreach(QAbstractButton *button, ui->buttonGroup->buttons())
    {
        button->setDisabled(true);
    }
    QFile file_stats("letter_stats.txt");
    file_stats.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_stats(&file_stats);
    while (!in_stats.atEnd())
    {

        QString line = in_stats.readLine();
        letter=line[0];
        QString letter_enabled = letter + "=1";
        QString letter_disabled = letter + "=0";
        index = line.indexOf('=');
        stats = line.mid(index+1).toFloat();
        if (stats>0.8)
        {
            dataText.replace(letter_disabled, letter_enabled);
        }
        else
        {
            dataText.replace(letter_enabled, letter_disabled);
        }
    }
    file_settings.close();
    in_temp << dataText;
    file_temp.close();
    file_settings.remove();
    file_temp.rename("settings.txt");
    foreach(QAbstractButton *button, ui->buttonGroup->buttons())
    {
        QString letter = button->objectName().right(1);
        QString letter_enabled = letter + "=1";
        QString letter_disabled = letter + "=0";
        if (dataText.contains(letter_enabled))
        {
            button->setStyleSheet("background-color: rgba(0, 255, 0, 0.4)");
        }
        else if (dataText.contains(letter_disabled))
        {
            button->setStyleSheet("background-color: rgba(255, 0, 0, 0.4)");
        }
    }
}

void Settings::on_button_manual_clicked()
{
    QFile file_settings("settings.txt");
    file_settings.open(QIODevice::ReadWrite | QIODevice::Text);
    QString dataText = file_settings.readAll();
    dataText.replace("letters_mode_=auto", "letters_mode_=manual");
    QFile file_temp("temp.txt");
    file_temp.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_temp(&file_temp);
    file_settings.close();
    in_temp << dataText;
    file_temp.close();
    file_settings.remove();
    file_temp.rename("settings.txt");
    foreach(QAbstractButton *button, ui->buttonGroup->buttons())
    {
        button->setEnabled(true);
    }
}

void Settings::on_button_strict_clicked()
{
    QFile file_settings("settings.txt");
    file_settings.open(QIODevice::ReadWrite | QIODevice::Text);
    QString dataText = file_settings.readAll();
    dataText.replace("typing_mode_=simple", "typing_mode_=strict");
    QFile file_temp("temp.txt");
    file_temp.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_temp(&file_temp);
    file_settings.close();
    in_temp << dataText;
    file_temp.close();
    file_settings.remove();
    file_temp.rename("settings.txt");
}

void Settings::on_button_simple_clicked()
{
    QFile file_settings("settings.txt");
    QFile file_temp("temp.txt");
    file_temp.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_temp(&file_temp);
    file_settings.open(QIODevice::ReadWrite | QIODevice::Text);
    QString dataText = file_settings.readAll();
    dataText.replace("typing_mode_=strict", "typing_mode_=simple");
    file_settings.close();
    in_temp << dataText;
    file_temp.close();
    file_settings.remove();
    file_temp.rename("settings.txt");
}

void Settings::on_slider_wordsamount_valueChanged(int value)
{
    QString amount_new = "words_amount_="+QString::number(value);
    QString dataText="";
    QFile file_settings("settings.txt");
    QFile file_temp("temp.txt");
    file_settings.open(QIODevice::ReadWrite | QIODevice::Text);
    file_temp.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_settings(&file_settings);
    QTextStream in_temp(&file_temp);
    while (!in_settings.atEnd())
    {
        QString line = in_settings.readLine();
        if (line.contains("words_amount_"))
            line.replace(line, amount_new);
        dataText+=line+'\n';
    }
    file_settings.close();
    dataText.chop(1);
    in_temp << dataText;
    file_temp.close();
    file_settings.remove();
    file_temp.rename("settings.txt");
    ui->label_wordsamount->setText("Words Amount: " + QString::number(value));
}

void Settings::on_checkbox_focus_clicked()
{
    QFile file_settings("settings.txt");
    file_settings.open(QIODevice::ReadWrite | QIODevice::Text);
    QString dataText = file_settings.readAll();
    if (dataText.contains("focus_mode_=0"))
    {
        dataText.replace("focus_mode_=0", "focus_mode_=1");
    }
    else if (dataText.contains("focus_mode_=1"))
    {
        dataText.replace("focus_mode_=1", "focus_mode_=0");
    }
    file_settings.seek(0);
    QTextStream out(&file_settings);
    out << dataText;
}

