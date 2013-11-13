#-------------------------------------------------
#
# Project created by QtCreator 2012-12-30T06:03:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HashCalculator
TEMPLATE = app

DESTDIR = $$DEST_BASE

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtGui/release/ -lPtGui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtGui/debug/ -lPtGui
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtGui/ -lPtGui

INCLUDEPATH += $$PWD/../../Library/PtGui
DEPENDPATH += $$PWD/../../Library/PtGui
