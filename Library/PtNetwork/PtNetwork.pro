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


win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$SRC_DIR/Library/PtCore/ -lPtCore

INCLUDEPATH += $$SRC_DIR/Library/PtCore
DEPENDPATH += $$SRC_DIR/Library/PtCore
