#-------------------------------------------------
#
# Project created by QtCreator 2013-04-21T22:52:17
#
#-------------------------------------------------

QT       -= gui

TARGET = PtLogging
TEMPLATE = lib

DEFINES += PTLOGGING_LIBRARY

SOURCES += \
    ptlogger.cpp

HEADERS +=\
        ptlogging_global.h \
    ptlogger.h

CONFIG += C++11

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
