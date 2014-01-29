#-------------------------------------------------
#
# Project created by QtCreator 2012-12-30T06:03:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HashCalculator
TEMPLATE = app

DESTDIR = $$BIN_DIR

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$SRC_DIR/Library/PtGui/release/ -lPtGui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SRC_DIR/Library/PtGui/debug/ -lPtGui
else:unix: LIBS += -L$$SRC_DIR/Library/PtGui/ -lPtGui

INCLUDEPATH += $$SRC_DIR/Library/PtGui
DEPENDPATH += $$SRC_DIR/Library/PtGui
