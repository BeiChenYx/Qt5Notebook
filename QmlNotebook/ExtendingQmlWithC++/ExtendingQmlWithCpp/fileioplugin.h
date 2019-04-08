#ifndef FILEIOPLUGIN_H
#define FILEIOPLUGIN_H

#include <QQuickItem>
#include <QQmlExtensionPlugin>

class FileioPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org-qt-project.Qt.QQmlExtensionInterface")
public:
    FileioPlugin();
    void registerTypes(const char *uri);
};

#endif // FILEIOPLUGIN_H
