#-------------------------------------------------
#
# Project created by QtCreator 2013-09-27T10:49:51
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = BaiduCloudConsole
CONFIG   += console C++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CapricornWorkerPlugins/BaiduCloudWorker/release/ -lBaiduCloudWorker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CapricornWorkerPlugins/BaiduCloudWorker/debug/ -lBaiduCloudWorker
else:unix: LIBS += -L$$OUT_PWD/../CapricornWorkerPlugins/BaiduCloudWorker/ -lBaiduCloudWorker

INCLUDEPATH += $$PWD/../CapricornWorkerPlugins/BaiduCloudWorker
DEPENDPATH += $$PWD/../CapricornWorkerPlugins/BaiduCloudWorker

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtCore/ -lPtCore

INCLUDEPATH += $$PWD/../../Library/PtCore
DEPENDPATH += $$PWD/../../Library/PtCore

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CapricornCore/release/ -lCapricornCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CapricornCore/debug/ -lCapricornCore
else:unix: LIBS += -L$$OUT_PWD/../CapricornCore/ -lCapricornCore

INCLUDEPATH += $$PWD/../CapricornCore
DEPENDPATH += $$PWD/../CapricornCore

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$PWD/../../Library/PtNetwork
DEPENDPATH += $$PWD/../../Library/PtNetwork
