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
    settingdialog.cpp

HEADERS  += mainwindow.h \
    settingdialog.h

FORMS    += mainwindow.ui \
    settingdialog.ui

RESOURCES += \
    buttonphotos.qrc
