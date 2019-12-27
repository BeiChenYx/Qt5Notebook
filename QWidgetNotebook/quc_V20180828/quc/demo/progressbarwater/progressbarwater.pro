#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T11:05:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = progressbarwater
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmprogressbarwater.cpp
SOURCES     += progressbarwater.cpp

HEADERS     += frmprogressbarwater.h
HEADERS     += progressbarwater.h

FORMS       += frmprogressbarwater.ui
