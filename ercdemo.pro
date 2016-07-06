#-------------------------------------------------
#
# Project created by QtCreator 2016-06-14T10:13:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ercdemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    statusdialog.cpp \
    logdialog.cpp \
    informationdialog.cpp \
    keyboard.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    statusdialog.h \
    logdialog.h \
    informationdialog.h \
    keyboard.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    statusdialog.ui \
    logdialog.ui \
    informationdialog.ui \
    keyboard.ui

RESOURCES += \
    buttonphotos.qrc

TRANSLATIONS += erc_demo_en.ts  \
    erc_demo_tw_zh.ts   \
    erc_demo_cn_zh.ts
