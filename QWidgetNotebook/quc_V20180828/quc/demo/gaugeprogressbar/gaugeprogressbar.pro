#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T10:37:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = gaugeprogressbar
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmgaugeprogressbar.cpp
SOURCES     += gaugeprogressbar.cpp

HEADERS     += frmgaugeprogressbar.h
HEADERS     += gaugeprogressbar.h

FORMS       += frmgaugeprogressbar.ui
