QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basecalculator.cpp \
    basedataloader.cpp \
    climatecsvparser.cpp \
    climaticparsingstrategy.cpp \
    climaticvariablesloader.cpp \
    connector.cpp \
    csvparser.cpp \
    csvwriter.cpp \
    database.cpp \
    datainterface.cpp \
    geography.cpp \
    geospatialcsvparser.cpp \
    geospatialdataloader.cpp \
    globaltypes.cpp \
    imageloader.cpp \
    landscapecalculator.cpp \
    main.cpp \
    mainwindow.cpp \
    matrixcalculator.cpp \
    nuclidesparser.cpp \
    rp5strategy.cpp \
    smithparamcalculator.cpp \
    sourcesloader.cpp

HEADERS += \
    basecalculator.h \
    basedataloader.h \
    climatecsvparser.h \
    climaticparsingstrategy.h \
    climaticvariablesloader.h \
    connector.h \
    csvparser.h \
    csvwriter.h \
    database.h \
    datainterface.h \
    geography.h \
    geospatialcsvparser.h \
    geospatialdataloader.h \
    globaltypes.h \
    imageloader.h \
    landscapecalculator.h \
    mainwindow.h \
    matrixcalculator.h \
    nuclidesparser.h \
    rp5strategy.h \
    smithparamcalculator.h \
    sourcesloader.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    scattering_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
