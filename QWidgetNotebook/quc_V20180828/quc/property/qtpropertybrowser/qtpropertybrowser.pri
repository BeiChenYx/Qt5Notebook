DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

RESOURCES += \
    $$PWD/qtpropertybrowser.qrc

HEADERS += \
    $$PWD/objectcontroller.h \
    $$PWD/qtbuttonpropertybrowser.h \
    $$PWD/qteditorfactory.h \
    $$PWD/qtgroupboxpropertybrowser.h \
    $$PWD/qtpropertybrowser.h \
    $$PWD/qtpropertybrowserutils_p.h \
    $$PWD/qtpropertymanager.h \
    $$PWD/qttreepropertybrowser.h \
    $$PWD/qtvariantproperty.h

SOURCES += \
    $$PWD/objectcontroller.cpp \
    $$PWD/qtbuttonpropertybrowser.cpp \
    $$PWD/qteditorfactory.cpp \
    $$PWD/qtgroupboxpropertybrowser.cpp \
    $$PWD/qtpropertybrowser.cpp \
    $$PWD/qtpropertybrowserutils.cpp \
    $$PWD/qtpropertymanager.cpp \
    $$PWD/qttreepropertybrowser.cpp \
    $$PWD/qtvariantproperty.cpp
