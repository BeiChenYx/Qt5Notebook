#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T15:39:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = navprogressbar
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmnavprogressbar.cpp
SOURCES     += navprogressbar.cpp

HEADERS     += frmnavprogressbar.h
HEADERS     += navprogressbar.h

FORMS       += frmnavprogressbar.ui

RESOURCES   += main.qrc
