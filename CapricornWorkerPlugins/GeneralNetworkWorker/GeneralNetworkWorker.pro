#-------------------------------------------------
#
# Project created by QtCreator 2013-08-02T17:02:06
#
#-------------------------------------------------

QT       -= gui

QT += network

CONFIG += C++11

TARGET = GeneralNetworkWorker
TEMPLATE = lib

DEFINES += GENERALNETWORKWORKER_LIBRARY

SOURCES += GeneralNetworkWorker.cpp

HEADERS += GeneralNetworkWorker.h\
        generalnetworkworker_global.h

DLLDESTDIR = $$PWD/../../bin

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../CapricornCore/release/ -lCapricornCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../CapricornCore/debug/ -lCapricornCore
else:unix: LIBS += -L$$OUT_PWD/../../CapricornCore/ -lCapricornCore

INCLUDEPATH += $$PWD/../../CapricornCore
DEPENDPATH += $$PWD/../../CapricornCore
