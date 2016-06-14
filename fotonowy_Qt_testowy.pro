#-------------------------------------------------
#
# Project created by QtCreator 2016-03-13T22:05:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fotonowy_Qt_testowy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    generator.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    generator.h \
    pcg-cpp-0.98/include/pcg_extras.hpp \
    pcg-cpp-0.98/include/pcg_random.hpp \
    qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    README.md

QMAKE_CXXFLAGS += -std=c++11
