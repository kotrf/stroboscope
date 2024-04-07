include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

SOURCES += \
    main.cpp \
    tst_main.cpp \
    ../../firmware/main/cmd.c

HEADERS +=
    ../../firmware/main/cmd.h


INCLUDEPATH += ../../firmware/main
