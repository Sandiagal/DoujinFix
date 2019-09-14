#-------------------------------------------------
#
# Project created by QtCreator 2019-05-06T21:03:55
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = DoujinFix
TEMPLATE = app

VERSION = 3.04

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
##DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    custom/draglineedit.cpp \
        main.cpp \
        mainwindow.cpp \
        custom/loglistmodel.cpp \
        setting/setting.cpp \
    file/standardiser.cpp \
    file/compressor.cpp \
    file/baseFile.cpp

HEADERS += \
    custom/draglineedit.h \
        mainwindow.h \
        custom/loglistmodel.h \
        setting/setting.h \
    file/standardiser.h \
    file/compressor.h \
    file/baseFile.h

FORMS += \
        mainwindow.ui

include (QSimpleUpdater/QSimpleUpdater.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc

RC_ICONS += "ico\Console.ico"

DISTFILES +=

TRANSLATIONS = zh_CN.ts
