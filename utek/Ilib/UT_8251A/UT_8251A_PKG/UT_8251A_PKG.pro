#-------------------------------------------------
#
# Project created by QtCreator 2020-04-01T11:41:47
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UT_8251A
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        connectingdevices.cpp \
        datatest.cpp \
        handlehid.cpp \
        handleserialport.cpp \
        handleserialprotocol.cpp \
        main.cpp \
        mainwindow.cpp\
        FrameLessWidget/framelesswidget.cpp \
        setfilter.cpp \
        setparameter.cpp \
        upgradedevice.cpp

HEADERS += \
        connectingdevices.h \
        datatest.h \
        handlehid.h \
        handleserialport.h \
        handleserialprotocol.h \
        setfilter.h \
        setparameter.h \
        upgradedevice.h \
        mainwindow.h\
        FrameLessWidget/framelesswidget.h

FORMS += \
        connectingdevices.ui \
        datatest.ui\
        mainwindow.ui\
        FrameLessWidget/framelesswidget.ui \
        setfilter.ui \
        setparameter.ui \
        upgradedevice.ui

#unix {
#    SOURCES += hid.c \
#}

#unix {
#LIBS += -ludev
#}

#win32 {
#LIBS += -L./ \
#        -lhidapi
#}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RC_FILE += FrameLessWidget/images/window.rc

RESOURCES += FrameLessWidget/images.qrc



unix|win32: LIBS += -L$$PWD/../LIB -lUT_8251A_LIB

INCLUDEPATH += $$PWD/../LIB
DEPENDPATH += $$PWD/../LIB

