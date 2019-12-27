#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "connectdlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnDlg dialog;
    if(dialog.exec() != QDialog::Accepted)
        return -1;
    QFile *carDetails = new QFile("attribs.xml");
    MainWindow w("factory", "cars", carDetails);
    w.show();

    return a.exec();
}
