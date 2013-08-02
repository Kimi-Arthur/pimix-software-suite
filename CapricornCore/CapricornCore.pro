#-------------------------------------------------
#
# Project created by QtCreator 2013-08-02T16:39:55
#
#-------------------------------------------------

QT       -= gui

TARGET = CapricornCore
TEMPLATE = lib

DEFINES += CAPRICORNCORE_LIBRARY

SOURCES += CapricornCore.cpp

HEADERS += CapricornCore.h\
        capricorncore_global.h \
    BaseCapricornWorker.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
