#-------------------------------------------------
#
# Project created by QtCreator 2020-02-24T09:55:01
#
#-------------------------------------------------

QT       += core gui network widgets sql charts

LIBS     += -Ldll -lws2_32
LIBS     += -lwsock32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UT_SF307_V160
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
        Libmodbus/modbus-data.c \
        Libmodbus/modbus-rtu.c \
        Libmodbus/modbus-tcp.c \
        Libmodbus/modbus.c \
        bartonwidget.cpp \
        customdialog.cpp \
        datarecordwidget.cpp \
        handlemodbus.cpp \
        homepage.cpp \
        hoverinfowidget.cpp \
        leftbartonwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        mbcrc.cpp \
        navprogressbar.cpp \
        parsetfive.cpp \
        parsetfour.cpp \
        parsetone.cpp \
        parsetsix.cpp \
        parsetthree.cpp \
        parsettwo.cpp \
        rightbartonwidget.cpp \
        scandevices.cpp \
        statuswidget.cpp \
        textmovewidget.cpp \
        tumbler.cpp \
        updatedevice.cpp

HEADERS += \
        Libmodbus/config.h \
        Libmodbus/modbus-private.h \
        Libmodbus/modbus-rtu-private.h \
        Libmodbus/modbus-rtu.h \
        Libmodbus/modbus-tcp-private.h \
        Libmodbus/modbus-tcp.h \
        Libmodbus/modbus-version.h \
        Libmodbus/modbus.h \
        bartonwidget.h \
        customdialog.h \
        datarecordwidget.h \
        handlemodbus.h \
        homepage.h \
        hoverinfowidget.h \
        leftbartonwidget.h \
        mainwindow.h \
        mbcrc.h \
        navprogressbar.h \
        parsetfive.h \
        parsetfour.h \
        parsetone.h \
        parsetsix.h \
        parsetthree.h \
        parsettwo.h \
        rightbartonwidget.h \
        scandevices.h \
        statuswidget.h \
        textmovewidget.h \
        tumbler.h \
        updatedevice.h

FORMS += \
        bartonwidget.ui \
        customdialog.ui \
        datarecordwidget.ui \
        homepage.ui \
        hoverinfowidget.ui \
        leftbartonwidget.ui \
        mainwindow.ui \
        parsetfive.ui \
        parsetfour.ui \
        parsetone.ui \
        parsetsix.ui \
        parsetthree.ui \
        parsettwo.ui \
        rightbartonwidget.ui \
        scandevices.ui \
        statuswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    font.qrc \
    img_v160.qrc

RC_FILE += SF.rc
