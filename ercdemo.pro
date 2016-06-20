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
    statusdialog.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    statusdialog.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    statusdialog.ui

RESOURCES += \
    buttonphotos.qrc
