QT += testlib

CONFIG += c++17

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SRC_DIR = ../src

INCLUDEPATH +=  $$SRC_DIR

SOURCES += \
    tst_scattering_test.cpp \
    $$SRC_DIR/csvwriter.cpp \
    $$SRC_DIR/meteorology.cpp \

HEADERS += \
    $$SRC_DIR/csvwriter.h \
    $$SRC_DIR/meteorology.h \

DEFINES += "TESTING"
