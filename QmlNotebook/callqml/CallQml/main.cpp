#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

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
        if(rootObjects.at(i)->objectName() == "rootObject")
        {
            root = rootObjects.at(i);
            break;
        }
    }

    QVariant returnedValue;
    QVariant msg1 = "Hello from C++";
    QMetaObject::invokeMethod(root, "myQmlFunction",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, msg1));
    qDebug() << "QML function returned: " << returnedValue.toString();

    return app.exec();
}
