#-------------------------------------------------
#
# Project created by QtCreator 2013-10-30T00:36:17
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_PtCoreTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_PtCoreTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PtCore/release/ -lPtCore -lz
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PtCore/debug/ -lPtCore -lz
else:unix: LIBS += -L$$OUT_PWD/../PtCore/ -lPtCore -lz

INCLUDEPATH += $$PWD/../PtCore
DEPENDPATH += $$PWD/../PtCore

