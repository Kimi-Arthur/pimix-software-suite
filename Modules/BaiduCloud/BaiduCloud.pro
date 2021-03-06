#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T20:01:50
#
#-------------------------------------------------

QT       += network concurrent

QT       -= gui

TARGET = BaiduCloud
TEMPLATE = lib

DEFINES += BAIDUCLOUD_LIBRARY

SOURCES += \
    BaiduCloudAccount.cpp \
    BaiduCloudService.cpp \
    BaiduCloudUploadException.cpp


PRECOMPILED_HEADER = BaiduCloud_global.h

HEADERS +=\
    BaiduCloudAccount.h \
    BaiduCloudService.h \
    BaiduCloudUploadException.h

DLLDESTDIR = $$BIN_DIR

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

win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$SRC_DIR/Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$SRC_DIR/Library/PtNetwork
DEPENDPATH += $$SRC_DIR/Library/PtNetwork
