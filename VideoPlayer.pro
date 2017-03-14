#-------------------------------------------------
#
# Project created by QtCreator 2017-03-03T12:25:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer
TEMPLATE = app
CONFIG 	   += c++11
QT += widgets core network

SOURCES += main.cpp\
        mainwindow.cpp \
    videoplayer.cpp

HEADERS  += mainwindow.h \
    videoplayer.h

FORMS    += mainwindow.ui

LIBS       += -lVLCQtCore -lVLCQtWidgets


LIBS       += -LC:\Qt\libvlc-qt\lib -lVLCQtCore -lVLCQtWidgets
INCLUDEPATH += C:\Qt\libvlc-qt\include
