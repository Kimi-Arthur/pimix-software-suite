#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T18:53:05
#
#-------------------------------------------------

QT       -= gui

TARGET = PtCore
TEMPLATE = lib

CONFIG += precompile_header

PRECOMPILED_HEADER = PtCore_global.h

DEFINES += PTCORE_LIBRARY

SOURCES += PtCore.cpp \
    PSettings.cpp \
    PFile.cpp \
    PConsole.cpp \
    PJsonValue.cpp \
    PLogger.cpp \
    PSerializer.cpp \
    PChecksum.cpp

HEADERS += PtCore.h\
    PSettings.h \
    PFile.h \
    PConsole.h \
    PJsonValue.h \
    PLogger.h \
    PSerializer.h \
    PChecksum.h \
    PtCore_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
