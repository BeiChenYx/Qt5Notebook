#-------------------------------------------------
#
# Project created by QtCreator 2019-05-16T20:16:21
#
#-------------------------------------------------

QT       += core gui sql charts
CONFIG += c++11
LIBS += -lpthread libwsock32 libws2_32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HumitureQWidget
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
        IM_modbus.cpp \
        Libmodbus/modbus-data.c \
        Libmodbus/modbus-rtu.c \
        Libmodbus/modbus-tcp.c \
        Libmodbus/modbus.c \
        comconfig.cpp \
        doubledisplay.cpp \
        home.cpp \
        humiturecharts.cpp \
        main.cpp \
        humiture.cpp \
        modbustest.cpp \
        modifycmd.cpp \
        readcmd.cpp \
        singleconfig.cpp \
        singledisplay.cpp

HEADERS += \
        IM_modbus.h \
        Libmodbus/config.h \
        Libmodbus/modbus-private.h \
        Libmodbus/modbus-rtu-private.h \
        Libmodbus/modbus-rtu.h \
        Libmodbus/modbus-tcp-private.h \
        Libmodbus/modbus-tcp.h \
        Libmodbus/modbus-version.h \
        Libmodbus/modbus.h \
        comconfig.h \
        doubledisplay.h \
        home.h \
        humiture.h \
        humiturecharts.h \
        modbustest.h \
        modifycmd.h \
        readcmd.h \
        singleconfig.h \
        singledisplay.h

FORMS += \
        comconfig.ui \
        doubledisplay.ui \
        home.ui \
        humiture.ui \
        modbustest.ui \
        modifycmd.ui \
        readcmd.ui \
        singleconfig.ui \
        singledisplay.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
