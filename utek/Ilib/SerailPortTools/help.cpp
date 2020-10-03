#include "help.h"
#include "ui_help.h"

help::help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);
    QFile file("help.html");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray text = file.readAll();
    ui->textBrowser->setHtml(QString(text));
    ui->textBrowser->setOpenExternalLinks(true);
}

help::~help()
{
    delete ui;
}
