#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T00:33:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11
TARGET = HttpEmulator
TEMPLATE = app

DESTDIR = $$DEST_BASE

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$PWD/../../Library/PtNetwork
DEPENDPATH += $$PWD/../../Library/PtNetwork


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtGui/release/ -lPtGui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtGui/debug/ -lPtGui
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtGui/ -lPtGui

INCLUDEPATH += $$PWD/../../Library/PtGui
DEPENDPATH += $$PWD/../../Library/PtGui
