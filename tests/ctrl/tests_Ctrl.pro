QT += testlib serialport
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_Ctrl.cpp \
    ../../ctrl/device.cpp

HEADERS += \
    ../../ctrl/device.h

INCLUDEPATH += \
    ../../ctrl/
