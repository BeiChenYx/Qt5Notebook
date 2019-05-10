QT += charts qml quick
CONFIG += c++11

LIBS     += -Ldll -lws2_32
LIBS     += -lwsock32

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        IM_modbus.cpp \
        Libmodbus/modbus-data.c \
        Libmodbus/modbus-rtu.c \
        Libmodbus/modbus-tcp.c \
        Libmodbus/modbus.c \
        communicatdef.cpp \
        handlehumiture.cpp \
        main.cpp

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    IM_modbus.h \
    Libmodbus/config.h \
    Libmodbus/modbus-private.h \
    Libmodbus/modbus-rtu-private.h \
    Libmodbus/modbus-rtu.h \
    Libmodbus/modbus-tcp-private.h \
    Libmodbus/modbus-tcp.h \
    Libmodbus/modbus-version.h \
    Libmodbus/modbus.h \
    communicatdef.h \
    handlehumiture.h
