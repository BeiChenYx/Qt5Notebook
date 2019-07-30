TEMPLATE	= app
CONFIG += c++11

#CONFIG		+= console
QT += widgets charts


#INCLUDEPATH += $$TRAIN_INCLUDE_PATH
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET	= ks02_07
TEMPLATE = app
FORMS		+= dialogbase.ui

HEADERS		+= \
            dialog.h \
            tip.h \
            chartview.h


SOURCES    +=  main.cpp \
            dialog.cpp \
            tip.cpp \
            chartview.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
