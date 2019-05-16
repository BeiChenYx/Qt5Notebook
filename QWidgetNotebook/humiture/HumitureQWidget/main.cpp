#include "humiture.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Humiture w;
    w.show();

    return a.exec();
}
