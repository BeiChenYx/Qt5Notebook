#-------------------------------------------------
#
# Project created by QtCreator 2017-08-10T17:11:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = ipaddress
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmipaddress.cpp
SOURCES     += ipaddress.cpp
SOURCES     += tumbler.cpp
SOURCES     += tumblerdate.cpp
SOURCES     += tumblertime.cpp
SOURCES     += tumblerdatetime.cpp
SOURCES     += combobox.cpp

HEADERS     += frmipaddress.h
HEADERS     += ipaddress.h
HEADERS     += tumbler.h
HEADERS     += tumblerdate.h
HEADERS     += tumblertime.h
HEADERS     += tumblerdatetime.h
HEADERS     += combobox.h

FORMS       += frmipaddress.ui
