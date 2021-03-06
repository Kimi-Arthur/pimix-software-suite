#-------------------------------------------------
#
# Project created by QtCreator 2012-12-30T06:30:26
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PtGui
TEMPLATE = lib

DEFINES += PTGUI_LIBRARY

DLLDESTDIR = $$BIN_DIR

SOURCES += \
    PFileChooserWidget.cpp

HEADERS +=\
        PtGui_global.h \
    PFileChooserWidget.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = $$LIB_DIR
    }
    INSTALLS += target
}

FORMS += \
    PFileChooserWidget.ui
