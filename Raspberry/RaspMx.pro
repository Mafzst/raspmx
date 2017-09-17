#-------------------------------------------------
#
# Project created by QtCreator 2015-12-13T16:40:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RaspMx
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        console.cpp \
		manual.cpp \
		spimanager.cpp \
    prepa.cpp

HEADERS  += mainwindow.h \
        console.h \
		manual.h \
		spimanager.h \
    prepa.h

FORMS    += mainwindow.ui \
        console.ui \
		manual.ui \
    prepa.ui

RESOURCES += style.qrc
