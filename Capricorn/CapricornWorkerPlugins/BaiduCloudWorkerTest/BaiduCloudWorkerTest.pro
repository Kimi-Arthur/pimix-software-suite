#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T20:10:08
#
#-------------------------------------------------

QT       += testlib network

QT       -= gui

TARGET = tst_BaiduCloudWorkerTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_BaiduCloudWorkerTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BaiduCloudWorker/release/ -lBaiduCloudWorker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BaiduCloudWorker/debug/ -lBaiduCloudWorker
else:unix: LIBS += -L$$OUT_PWD/../BaiduCloudWorker/ -lBaiduCloudWorker

INCLUDEPATH += $$PWD/../BaiduCloudWorker
DEPENDPATH += $$PWD/../BaiduCloudWorker

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$OUT_PWD/../../../Library/PtCore/ -lPtCore

INCLUDEPATH += $$PWD/../../../Library/PtCore
DEPENDPATH += $$PWD/../../../Library/PtCore

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../CapricornCore/release/ -lCapricornCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../CapricornCore/debug/ -lCapricornCore
else:unix: LIBS += -L$$OUT_PWD/../../CapricornCore/ -lCapricornCore

INCLUDEPATH += $$PWD/../../CapricornCore
DEPENDPATH += $$PWD/../../CapricornCore

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$OUT_PWD/../../../Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$PWD/../../../Library/PtNetwork
DEPENDPATH += $$PWD/../../../Library/PtNetwork
