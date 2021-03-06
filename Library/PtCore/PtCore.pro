#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T18:53:05
#
#-------------------------------------------------

QT       -= gui

TARGET = PtCore
TEMPLATE = lib

DLLDESTDIR = $$BIN_DIR

PRECOMPILED_HEADER = PtCore_global.h

DEFINES += PTCORE_LIBRARY

SOURCES += PtCore.cpp \
    PSettings.cpp \
    PFile.cpp \
    PConsole.cpp \
    PJsonValue.cpp \
    PLogger.cpp \
    PSerializer.cpp \
    PChecksum.cpp \
    PString.cpp \
    PConfig.cpp \
    PDataModel.cpp

HEADERS += PtCore.h\
    PSettings.h \
    PFile.h \
    PConsole.h \
    PJsonValue.h \
    PLogger.h \
    PSerializer.h \
    PChecksum.h \
    PtCore_global.h \
    PString.h \
    PConfig.h \
    PDataModel.h

LIBS += -lz

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = $$LIB_DIR
    }
    INSTALLS += target
}
