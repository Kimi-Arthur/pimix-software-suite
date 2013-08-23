#-------------------------------------------------
#
# Project created by QtCreator 2013-08-23T23:13:00
#
#-------------------------------------------------

QT       -= gui

TARGET = PtCore
TEMPLATE = lib

DEFINES += PTCORE_LIBRARY

SOURCES += PtCore.cpp \
    PLogger.cpp

HEADERS += PtCore.h\
        ptcore_global.h \
    PLogger.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
