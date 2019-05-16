#include <QQmlApplicationEngine>
#include <QtWidgets/QApplication>
#include <QtQuick/QQuickView>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>
#include "handlehumiture.h"
#include "realhumiture.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qmlRegisterType<HandleHumiture>("an.qt.HandleHumiture", 1, 0, "HandleHumiture");
    qmlRegisterType<HandleHumiture>("an.qt.RealHumiture", 1, 0, "RealHumiture");


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject *root = NULL;
    QList<QObject*> rootObjects = engine.rootObjects();
    for (int i = 0; i < rootObjects.size(); i++) {
        if(rootObjects.at(i)->objectName() == "singleObjects")
        {
            root = rootObjects.at(i);
            break;
        }
    }

    return app.exec();
}
