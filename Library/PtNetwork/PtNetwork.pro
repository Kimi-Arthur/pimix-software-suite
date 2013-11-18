#-------------------------------------------------
#
# Project created by QtCreator 2012-12-28T10:30:28
#
#-------------------------------------------------

QT       += network

QT       -= gui

DLLDESTDIR = $$BIN_DIR

TARGET = PtNetwork
TEMPLATE = lib

DEFINES += PTNETWORK_LIBRARY

SOURCES += PtNetwork.cpp \
    PNetworkAccessManager.cpp \
    PNetworkRetryPolicy.cpp

HEADERS += PtNetwork.h\
        PtNetwork_global.h \
    PNetworkAccessManager.h \
    PNetworkRetryPolicy.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = $$LIB_DIR
    }
    INSTALLS += target
}
