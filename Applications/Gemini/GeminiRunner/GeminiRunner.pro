#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T11:47:32
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = GeminiRunner
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $$BIN_DIR

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Modules/BaiduCloudWorker/release/ -lBaiduCloudWorker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Modules/BaiduCloudWorker/debug/ -lBaiduCloudWorker
else:unix: LIBS += -L$$SRC_DIR/Modules/BaiduCloudWorker/ -lBaiduCloudWorker

INCLUDEPATH += $$SRC_DIR/Modules/BaiduCloudWorker
DEPENDPATH += $$SRC_DIR/Modules/BaiduCloudWorker

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
