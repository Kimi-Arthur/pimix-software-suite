#-------------------------------------------------
#
# Project created by QtCreator 2013-11-05T00:06:23
#
#-------------------------------------------------

QT       -= gui

TARGET = untitled
TEMPLATE = lib

DEFINES += UNTITLED_LIBRARY

SOURCES += Untitled.cpp

HEADERS += Untitled.h\
        untitled_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
