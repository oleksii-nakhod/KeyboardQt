#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "info.h"
#include <QFile>
#include <string>
#include <iostream>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QVector>
#include <QKeyEvent>
#include <QSignalBlocker>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    srand(time(NULL));
    ui->setupUi(this);
    reload();
    ui->label_text->setText(text);
    ui->inputbox->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inputbox_textChanged()
{
    if (words_arr.length()==0)
    {
        return;
    }
    if (cursor_index==0)
    {
        strict_error=0;
        return;
    }

    QString input = ui->inputbox->toPlainText();

    if (input[0]==' ')
    {
        cursor_index=0;
        ui->inputbox->setText("");
        return;
    }

    int input_size_new=input.size();

    if (input_size_new<input_size)
    {
        input_size=input_size_new;
        strict_error=0;
        return;
    }

    if (cursor_index==text.size())
    {
        QFile file_write("wpm_stats.txt");
        file_write.open(QIODevice::Append | QIODevice::Text);
        QTextStream out(&file_write);
        out << wpm << "\n";
        file_write.close();
        if (letter_focus!=" ")
        {
            float old_stats, new_stats;
            QString dataText;
            QFile file("letter_stats.txt");
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            QTextStream in(&file);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (line.contains(letter_focus))
                {
                    int index = line.indexOf('=');
                    old_stats = line.mid(index+1).toFloat();
                    new_stats = calculate_new_stats(old_stats);
                    line.replace(QString::number(old_stats), QString::number(new_stats));
                }
                dataText += line + '\n';
            }
            dataText.chop(1);
            file.seek(0);
            in << dataText;
        }

        reload();
        return;
    }

    if (start_timer == 0)
    {
        start_timer = 1;
        start_clock = clock();
        connect(timer, SIGNAL(timeout()), this, SLOT(update_values()));
        timer->start(1000);
    }

    if (strict==1 && strict_error==1)
    {
        bool oldState = ui->inputbox->blockSignals(true);
        ui->inputbox->textCursor().deletePreviousChar();
        ui->inputbox->blockSignals(oldState);
        ui->inputbox->moveCursor(QTextCursor::End);
        return;
    }

    input_size=input_size_new;

    if (input[cursor_index-1]!=text[cursor_index-1])
    {
        error_count++;
        strict_error=1;
    }
    else
    {
        correct_count++;
    }

    ui->label_errors->setText("errors: " + QString::number(error_count));
}

void MainWindow::update_values()
{
    float elapsed = clock()-start_clock;
    if (elapsed<300)
        return;
    wpm = (float(correct_count)/5)/(elapsed/1000/60);
    ui->label_wpm->setText("WPM: " + QString::number(round(wpm)));
}

void MainWindow::on_inputbox_cursorPositionChanged()
{
    cursor_index = ui->inputbox->textCursor().position();
}

void MainWindow::reload()
{
    srand(time(NULL));
    words_focus_arr.clear();
    error_count = 0;
    correct_count = 0;
    cursor_index = 0;
    letter_index = 0;
    input_size = 0;
    start_timer = 0;
    strict_error = 0;
    wpm = 0;
    text.clear();
    timer->stop();
    words_arr=create_words_array();
    if (focus_mode == 1)
    {
        for (int i=0; i<words_arr.length(); i++)
        {
            if (words_arr[i].contains(letter_focus))
                    words_focus_arr.push_back(words_arr[i]);
        }
    }
    for (int i=0; i<words_amount; i++)
    {
        if (words_arr.length()==0)
        {
            text="<not enough letters selected> ";
            break;
        }
        if (focus_mode == 1)
            text += words_focus_arr[rand()%words_focus_arr.length()] + " ";
        if (focus_mode == 0)
            text += words_arr[rand()%words_arr.length()] + " ";

    }
    text.chop(1);
    ui->label_text->setText(text);
    ui->inputbox->setText("");
    ui->label_errors->setText("errors: " + QString::number(error_count));
    ui->label_wpm->setText("WPM: " + QString::number(round(wpm)));
    ui->inputbox->setFocus();
    ui->label_focus->setText("letter focus: " + letter_focus);
    if (focus_mode==1)
    {
        ui->label_focus->show();
    }
    if (focus_mode==0)
    {
        ui->label_focus->hide();
    }
}

void MainWindow::on_actionProperties_triggered()
{
    settings.show();
}

void MainWindow::on_actionReload_triggered()
{
    reload();
}

void MainWindow::on_button_settings_clicked()
{
    settings.show();
}

void MainWindow::on_button_reload_clicked()
{
    reload();
}

void MainWindow::on_button_info_clicked()
{
    info.show();
}

float MainWindow::calculate_new_stats(float old_stats)
{
    float score = wpm-error_count*3;
    float new_stats = old_stats;
    new_stats += (score-25)*words_amount/2000;
    if (new_stats<0)
        new_stats=0;
    if (new_stats>1)
        new_stats=1;
    return new_stats;
}

QVector<QString> MainWindow::create_words_array()
{

    float stats, stats_min=1;
    int index=0;
    QString letter, dataText;
    QString allowed_letters_new="";
    QFile file_settings("settings.txt");
    file_settings.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_settings(&file_settings);
    while (!in_settings.atEnd())
    {
        QString line = in_settings.readLine();

        if (line.contains("typing_mode_=strict"))
        {
            strict = 1;
        }
        else if (line.contains("typing_mode_=simple"))
        {
            strict = 0;
        }
        else if (line.contains("words_amount_"))
        {
            int index = line.indexOf('=');
            words_amount = line.mid(index+1).toInt();
        }
        else if (line.contains("focus_mode_=1"))
        {
            focus_mode=1;
        }
        else if (line.contains("focus_mode_=0"))
        {
            focus_mode=0;
        }
        else if (line.contains("letters_mode_=auto"))
        {
            letters_mode=0;
        }
        else if (line.contains("letters_mode_=manual"))
        {
            letters_mode=1;
        }
        else if (line.right(2)=="=1" && letters_mode==1)
            allowed_letters_new  += line[0];
    }

    file_settings.seek(0);
    dataText = file_settings.readAll();

    QFile file_stats("letter_stats.txt");
    file_stats.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in_stats(&file_stats);
    while (!in_stats.atEnd())
    {
        QString line = in_stats.readLine();
        letter=line[0];

        if (!allowed_letters_new.contains(letter))
            continue;

        index = line.indexOf('=');
        stats = line.mid(index+1).toFloat();
        if (stats<=stats_min)
        {
            stats_min=stats;
            letter_focus=letter;
        }
    }

    file_stats.seek(0);
    if (letters_mode==0)
    {

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
                allowed_letters_new += line[0];
                dataText.replace(letter_disabled, letter_enabled);
            }
            else
            {
                dataText.replace(letter_enabled, letter_disabled);
            }
        }
        for (int i=0; i<frequency_order.length(); i++)
        {
            if (!allowed_letters_new.contains(frequency_order[i]))
            {
                QString letter = frequency_order[i];
                allowed_letters_new+=letter;
                dataText.replace(letter+"=0", letter+"=1");
                break;
            }
        }
        file_settings.seek(0);
        QTextStream out(&file_settings);
        out << dataText;
    }

    if (QString::compare(allowed_letters, allowed_letters_new)!=0)
    {

        allowed_letters = allowed_letters_new;
        words_arr.clear();
        QFile file_read(":/words_1000.txt");
        file_read.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file_read);
        int add_word = 1;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            for (int i=0; i<line.length(); i++)
            {
                if (!allowed_letters.contains(line.at(i)))
                {
                    add_word = 0;
                    continue;
                }
            }
            if (add_word)
            {
                words_arr.push_back(line);
            }
            add_word = 1;
        }
    }

    return words_arr;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

