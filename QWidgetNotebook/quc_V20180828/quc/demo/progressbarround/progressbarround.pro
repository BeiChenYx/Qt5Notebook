#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T15:59:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = progressbarround
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmprogressbarround.cpp
SOURCES     += progressbarround.cpp

HEADERS     += frmprogressbarround.h
HEADERS     += progressbarround.h

FORMS       += frmprogressbarround.ui
