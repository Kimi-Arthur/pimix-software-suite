#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T00:43:50
#
#-------------------------------------------------

QT       += core

QT       -= gui

CONFIG += C++11

TARGET = ConsoleCapricornRunner
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $$PWD/../bin

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CapricornCore/release/ -lCapricornCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CapricornCore/debug/ -lCapricornCore
else:unix: LIBS += -L$$OUT_PWD/../CapricornCore/ -lCapricornCore

INCLUDEPATH += $$PWD/../CapricornCore
DEPENDPATH += $$PWD/../CapricornCore
