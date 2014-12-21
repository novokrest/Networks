TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../common/

SOURCES += \
    s_main.cpp \
    server.cpp \
    s_simple.cpp \
    s_test.cpp \
    ../common/request.cpp \
    ../common/utils.cpp \
    ../common/logger.cpp \
    ../common/response.cpp

HEADERS += \
    server.h \
    ../common/common.h \
    ../common/errors.h \
    ../common/request.h \
    ../common/utils.h \
    ../common/logger.h \
    ../common/response.h

