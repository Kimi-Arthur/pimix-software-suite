#-------------------------------------------------
#
# Project created by QtCreator 2013-09-14T21:15:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FileInformationGenerator
CONFIG   += console C++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtCore/ -lPtCore

INCLUDEPATH += $$PWD/../../Library/PtCore
DEPENDPATH += $$PWD/../../Library/PtCore
