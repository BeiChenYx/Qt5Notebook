#include "frmgaugecar.h"
#include <QApplication>
#include <QTextCodec>
#include <QAxFactory>

QAXFACTORY_BEGIN(
    "{ce947ee3-0403-4fdc-895a-4fe779394b46}", // type library ID
    "{8de435ce-8d2a-46ac-b3b3-cb800d0847c5}") // application ID
QAXCLASS(frmGaugeCar)
QAXFACTORY_END()

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    QWidget *w = 0;
    if (!QAxFactory::isServer()) {
        w = new frmGaugeCar();
        w->show();
    }

    return a.exec();
}
