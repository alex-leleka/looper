#-------------------------------------------------
#
# Project created by QtCreator 2014-09-27T04:00:37
#
#-------------------------------------------------

QT       += core gui widgets multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoundRecordApp
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    audiorecordengine.cpp \
    audiorecordenginesettings.cpp \
    recordersettings.cpp

HEADERS  += mainwindow.h \
    audiorecordengine.h \
    audiorecordenginesettings.h \
    recordersettings.h
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui \
    recordersettings.ui

RESOURCES += \
    SoundRecordApp.qrc
