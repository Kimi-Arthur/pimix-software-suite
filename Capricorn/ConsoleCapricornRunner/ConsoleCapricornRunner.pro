#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T00:43:50
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ConsoleCapricornRunner
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $$BIN_DIR

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CapricornCore/release/ -lCapricornCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CapricornCore/debug/ -lCapricornCore
else:unix: LIBS += -L$$OUT_PWD/../CapricornCore/ -lCapricornCore

INCLUDEPATH += $$PWD/../CapricornCore
DEPENDPATH += $$PWD/../CapricornCore

win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$SRC_DIR/Library/PtCore/ -lPtCore

INCLUDEPATH += $$SRC_DIR/Library/PtCore
DEPENDPATH += $$SRC_DIR/Library/PtCore
