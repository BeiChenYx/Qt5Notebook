#-------------------------------------------------
#
# Project created by QtCreator 2016-12-17T11:38:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET                  = examples
TEMPLATE                = app
MOC_DIR                 = temp/moc
RCC_DIR                 = temp/rcc
UI_DIR                  = temp/ui
OBJECTS_DIR             = temp/obj
DESTDIR                 = $$PWD/../bin
PRECOMPILED_HEADER      = head.h
win32:RC_FILE           = other/main.rc

CONFIG                  += warn_off
RESOURCES               += other/main.qrc
INCLUDEPATH             += $$PWD/../designer/include

CONFIG(release, debug|release){
LIBS        += -L$$PWD/../bin/ -lquc
} else {
unix {LIBS  += -L$$PWD/../bin/ -lquc}
else {LIBS  += -L$$PWD/../bin/ -lqucd}
}

include     ($$PWD/frmgauge/frmgauge.pri)
include     ($$PWD/frmprogress/frmprogress.pri)
include     ($$PWD/frmnav/frmnav.pri)
include     ($$PWD/frmwave/frmwave.pri)
include     ($$PWD/frmruler/frmruler.pri)
include     ($$PWD/frmslider/frmslider.pri)
include     ($$PWD/frmbar/frmbar.pri)
include     ($$PWD/frmcolor/frmcolor.pri)
include     ($$PWD/frmimage/frmimage.pri)
include     ($$PWD/frmpainter/frmpainter.pri)
include     ($$PWD/frmother/frmother.pri)

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/frmgauge
INCLUDEPATH += $$PWD/frmprogress
INCLUDEPATH += $$PWD/frmnav
INCLUDEPATH += $$PWD/frmwave
INCLUDEPATH += $$PWD/frmruler
INCLUDEPATH += $$PWD/frmslider
INCLUDEPATH += $$PWD/frmbar
INCLUDEPATH += $$PWD/frmcolor
INCLUDEPATH += $$PWD/frmimage
INCLUDEPATH += $$PWD/frmpainter
INCLUDEPATH += $$PWD/frmother

FORMS += \
    frmmain.ui

HEADERS += \
    frmmain.h \
    iconhelper.h

SOURCES += \
    frmmain.cpp \
    iconhelper.cpp \
    main.cpp
