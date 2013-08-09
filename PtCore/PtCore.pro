#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T18:53:05
#
#-------------------------------------------------

QT       -= gui

TARGET = PtCore
TEMPLATE = lib

DEFINES += PTCORE_LIBRARY

SOURCES += PtCore.cpp \
    PSettings.cpp

HEADERS += PtCore.h\
        ptcore_global.h \
    PSettings.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
