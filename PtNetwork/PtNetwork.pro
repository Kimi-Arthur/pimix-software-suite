#-------------------------------------------------
#
# Project created by QtCreator 2012-12-28T10:30:28
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = PtNetwork
TEMPLATE = lib

DEFINES += PTNETWORK_LIBRARY

SOURCES += ptnetwork.cpp

HEADERS += ptnetwork.h\
        PtNetwork_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
