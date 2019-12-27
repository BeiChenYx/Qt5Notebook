#-------------------------------------------------
#
# Project created by QtCreator 2018-08-26T10:14:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = property
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

include     ($$PWD/qtcontrol/qtcontrol.pri)
include     ($$PWD/qtpropertybrowser/qtpropertybrowser.pri)

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/qtcontrol
INCLUDEPATH += $$PWD/qtpropertybrowser

SOURCES     += main.cpp
SOURCES     += frmproperty.cpp
HEADERS     += frmproperty.h
FORMS       += frmproperty.ui
RESOURCES   += main.qrc
