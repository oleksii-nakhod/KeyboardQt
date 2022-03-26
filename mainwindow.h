#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <settings.h>
#include <info.h>
#include <QMainWindow>
#include <QElapsedTimer>
#include <QVector>
#include <QTimer>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int error_count = 0;
    int correct_count = 0;
    int cursor_index = 0;
    int letter_index = 0;
    int input_size = 0;
    int start_timer = 0;
    int words_amount = 10;
    int strict = 0;
    int strict_error = 0;
    float wpm = 0;
    int focus_mode = 0;
    int letters_mode = 1;
    QFile file_read;
    QFile file_write;
    QTimer *timer = new QTimer(this);
    QString text = "some text";
    QString allowed_letters = "";
    QString frequency_order = "eariotnslcudpmhgbfywkvxzjq";
    QVector<QString> words_arr;
    QVector<QString> words_focus_arr;
    QString letter_focus = "e";
    clock_t start_clock;
private slots:
    void on_inputbox_textChanged();

    void on_inputbox_cursorPositionChanged();

    void on_actionProperties_triggered();

    void on_actionReload_triggered();

    void on_button_settings_clicked();

    void on_button_reload_clicked();

    void on_button_info_clicked();

    float calculate_new_stats(float old_stats);

    void on_actionExit_triggered();

public slots:
    void update_values();
    void reload();
    QVector<QString> create_words_array();
private:
    Ui::MainWindow *ui;
    Settings settings;
    Info info;
};


#endif // MAINWINDOW_H
