#-------------------------------------------------
#
# Project created by QtCreator 2014-09-27T04:00:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoundRecordApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audiorecordengine.cpp

HEADERS  += mainwindow.h \
    audiorecordengine.h

FORMS    += mainwindow.ui
