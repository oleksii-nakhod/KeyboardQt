#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

void initializeFiles();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initializeFiles();
    MainWindow w;
    w.show();
    return a.exec();
}

void initializeFiles() {
    QFile file_settings("settings.txt");

    if (!file_settings.exists() || file_settings.size() == 0) {
        if (!file_settings.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return;
        }

        QTextStream out(&file_settings);
        out << "typing_mode_=simple\n";
        out << "letters_mode_=auto\n";
        out << "words_amount_=10\n";
        out << "focus_mode_=0\n";
        out << "a=1\n";
        out << "b=0\n";
        out << "c=1\n";
        out << "d=1\n";
        out << "e=0\n";
        out << "f=0\n";
        out << "g=1\n";
        out << "h=0\n";
        out << "i=1\n";
        out << "j=1\n";
        out << "k=0\n";
        out << "l=0\n";
        out << "m=0\n";
        out << "n=1\n";
        out << "o=1\n";
        out << "p=0\n";
        out << "q=1\n";
        out << "r=0\n";
        out << "s=1\n";
        out << "t=1\n";
        out << "u=1\n";
        out << "v=0\n";
        out << "w=0\n";
        out << "x=0\n";
        out << "y=0\n";
        out << "z=0\n";

        file_settings.close();
    }

    QFile file_stats("letter_stats.txt");
    if (!file_stats.exists() || file_stats.size() == 0) {
        if (!file_stats.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return;
        }

        QTextStream out(&file_stats);
        out << "a=1\n";
        out << "b=0\n";
        out << "c=1\n";
        out << "d=1\n";
        out << "e=0\n";
        out << "f=0\n";
        out << "g=1\n";
        out << "h=0\n";
        out << "i=1\n";
        out << "j=1\n";
        out << "k=0\n";
        out << "l=0\n";
        out << "m=0\n";
        out << "n=1\n";
        out << "o=1\n";
        out << "p=0\n";
        out << "q=1\n";
        out << "r=0\n";
        out << "s=1\n";
        out << "t=1\n";
        out << "u=1\n";
        out << "v=0\n";
        out << "w=0\n";
        out << "x=0\n";
        out << "y=0\n";
        out << "z=0\n";

        file_stats.close();
    }
}
