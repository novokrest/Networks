TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    client.cpp \
    utils.cpp \
    request.cpp \
    server.cpp \
    s_main.cpp \
    c_main.cpp \
    test.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    client.h \
    utils.h \
    errors.h \
    common.h \
    server.h \
    request.h

