#-------------------------------------------------
#
# Project created by QtCreator 2019-05-28T16:14:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 360Safe
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    about_us.cpp \
    account_item.cpp \
    change_skin_widget.cpp \
    character_widget.cpp \
    clabel.cpp \
    content_widget.cpp \
    drop_shadow_widget.cpp \
    kill_mummy_widget.cpp \
    loading_widget.cpp \
    login_dialog.cpp \
    main.cpp \
    main_menu.cpp \
    main_widget.cpp \
    msg_box.cpp \
    push_button.cpp \
    register_widget.cpp \
    setting_dialog.cpp \
    shadow_widget.cpp \
    skin_widget.cpp \
    system_tray.cpp \
    title_widget.cpp \
    tool_button.cpp \
    util.cpp

HEADERS += \
    about_us.h \
    account_item.h \
    change_skin_widget.h \
    character_widget.h \
    clabel.h \
    common.h \
    content_widget.h \
    drop_shadow_widget.h \
    kill_mummy_widget.h \
    loading_widget.h \
    login_dialog.h \
    main_menu.h \
    main_widget.h \
    msg_box.h \
    push_button.h \
    register_widget.h \
    resource.h \
    setting_dialog.h \
    shadow_widget.h \
    skin_widget.h \
    system_tray.h \
    title_widget.h \
    tool_button.h \
    util.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    readMe.txt

RESOURCES += \
    Resources/360safe.qrc
