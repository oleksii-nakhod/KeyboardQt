#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    ui->label_info->setText("This application will help you learn touch typing, which means that you'll be able to find the keys on the keyboard through muscle memory without using the sense of sight. This skill can improve your typing speed and accuracy dramatically, as well as reduce neck strain and attention switching. Start by putting your fingers on the home row, as shown in the illustration, then type the text that appears on screen. Good luck!");
    ui->label_thanks->setText("Inspired by: <a href=\"http://www.keybr.com\">Keybr</a>\n<a href=\"http://www.monkeytype.com\">monkeytype</a>");
    ui->label_thanks->setOpenExternalLinks(true);
}

Info::~Info()
{
    delete ui;
}
