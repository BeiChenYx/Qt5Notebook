#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T10:14:09
#
#-------------------------------------------------

QT       += core gui axserver

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = qtactivex
TEMPLATE    = app
DESTDIR     = bin
RC_FILE     = main.rc
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmgaugecar.cpp
SOURCES     += gaugecar.cpp

HEADERS     += frmgaugecar.h
HEADERS     += gaugecar.h

FORMS       += frmgaugecar.ui

