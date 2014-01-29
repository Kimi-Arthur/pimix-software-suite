#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T00:33:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HttpEmulator
TEMPLATE = app

DESTDIR = $$BIN_DIR

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$SRC_DIR/Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$SRC_DIR/Library/PtNetwork
DEPENDPATH += $$SRC_DIR/Library/PtNetwork


win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Library/PtGui/release/ -lPtGui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Library/PtGui/debug/ -lPtGui
else:unix: LIBS += -L$$SRC_DIR/Library/PtGui/ -lPtGui

INCLUDEPATH += $$SRC_DIR/Library/PtGui
DEPENDPATH += $$SRC_DIR/Library/PtGui
