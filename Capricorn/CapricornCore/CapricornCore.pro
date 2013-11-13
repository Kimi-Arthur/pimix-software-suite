#-------------------------------------------------
#
# Project created by QtCreator 2013-08-02T16:39:55
#
#-------------------------------------------------

QT       -= gui

QT       += concurrent

TARGET = CapricornCore
TEMPLATE = lib

DEFINES += CAPRICORNCORE_LIBRARY

SOURCES += CapricornCore.cpp \
    BaseCapricornWorker.cpp

HEADERS += CapricornCore.h\
        capricorncore_global.h \
    CapricornWorker.h \
    BaseCapricornWorker.h

DLLDESTDIR = $$BIN_DIR

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$SRC_DIR/Library/PtCore/ -lPtCore

INCLUDEPATH += $$SRC_DIR/Library/PtCore
DEPENDPATH += $$SRC_DIR/Library/PtCore
