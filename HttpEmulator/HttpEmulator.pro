#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T00:33:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HttpEmulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$PWD/../../Library/PtNetwork
DEPENDPATH += $$PWD/../../Library/PtNetwork
