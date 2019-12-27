#-------------------------------------------------
#
# Project created by QtCreator 2017-01-07T17:11:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = progressbarpercent
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmprogressbarpercent.cpp
SOURCES     += progressbarpercent.cpp

HEADERS     += frmprogressbarpercent.h
HEADERS     += progressbarpercent.h

FORMS       += frmprogressbarpercent.ui

RESOURCES   += main.qrc
