#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T20:01:50
#
#-------------------------------------------------

QT       += network concurrent

QT       -= gui

TARGET = BaiduCloudWorker
TEMPLATE = lib

CONFIG += C++11

DEFINES += BAIDUCLOUDWORKER_LIBRARY

SOURCES += BaiduCloudWorker.cpp

HEADERS += BaiduCloudWorker.h\
        baiducloudworker_global.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$OUT_PWD/../../../Library/PtCore/ -lPtCore

INCLUDEPATH += $$PWD/../../../Library/PtCore
DEPENDPATH += $$PWD/../../../Library/PtCore

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$OUT_PWD/../../../Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$PWD/../../../Library/PtNetwork
DEPENDPATH += $$PWD/../../../Library/PtNetwork
