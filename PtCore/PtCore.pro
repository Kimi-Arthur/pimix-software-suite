#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T18:53:05
#
#-------------------------------------------------

QT       -= gui

CONFIG += C++11

TARGET = PtCore
TEMPLATE = lib

CONFIG += C++11

DEFINES += PTCORE_LIBRARY

SOURCES += PtCore.cpp \
    PSettings.cpp \
    PFile.cpp \
    PConsole.cpp

HEADERS += PtCore.h\
        ptcore_global.h \
    PSettings.h \
    PFile.h \
    PConsole.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
