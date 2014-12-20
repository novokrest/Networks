TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../common

SOURCES += \
    c_main.cpp \
    client.cpp \
    c_simple.cpp \
    c_test.cpp \
    ../common/request.cpp \
    ../common/utils.cpp

HEADERS += \
    client.h \
    ../common/common.h \
    ../common/errors.h \
    ../common/request.h \
    ../common/utils.h

