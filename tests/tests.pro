QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SRC_DIR = ../src

INCLUDEPATH +=  $$SRC_DIR

HEADERS += \
     $$SRC_DIR/meteorology.h

SOURCES += \
    tst_scattering_test.cpp \
    $$SRC_DIR/meteorology.cpp
