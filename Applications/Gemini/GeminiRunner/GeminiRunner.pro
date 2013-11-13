#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T11:47:32
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = GeminiRunner
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Capricorn/CapricornWorkerPlugins/BaiduCloudWorker/release/ -lBaiduCloudWorker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Capricorn/CapricornWorkerPlugins/BaiduCloudWorker/debug/ -lBaiduCloudWorker
else:unix: LIBS += -L$$OUT_PWD/../../Capricorn/CapricornWorkerPlugins/BaiduCloudWorker/ -lBaiduCloudWorker

INCLUDEPATH += $$PWD/../../Capricorn/CapricornWorkerPlugins/BaiduCloudWorker
DEPENDPATH += $$PWD/../../Capricorn/CapricornWorkerPlugins/BaiduCloudWorker

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtCore/release/ -lPtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtCore/debug/ -lPtCore
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtCore/ -lPtCore

INCLUDEPATH += $$PWD/../../Library/PtCore
DEPENDPATH += $$PWD/../../Library/PtCore

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/release/ -lPtNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Library/PtNetwork/debug/ -lPtNetwork
else:unix: LIBS += -L$$OUT_PWD/../../Library/PtNetwork/ -lPtNetwork

INCLUDEPATH += $$PWD/../../Library/PtNetwork
DEPENDPATH += $$PWD/../../Library/PtNetwork

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Capricorn/CapricornCore/release/ -lCapricornCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Capricorn/CapricornCore/debug/ -lCapricornCore
else:unix: LIBS += -L$$OUT_PWD/../../Capricorn/CapricornCore/ -lCapricornCore

INCLUDEPATH += $$PWD/../../Capricorn/CapricornCore
DEPENDPATH += $$PWD/../../Capricorn/CapricornCore
