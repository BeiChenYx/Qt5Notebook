#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T16:18:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = frmdemo
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

include     ($$PWD/frmdemo/frmdemo.pri)

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/frmdemo

SOURCES += main.cpp \
    frmdemo.cpp

FORMS += \
    frmdemo.ui

HEADERS += \
    frmdemo.h
