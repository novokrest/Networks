TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    request.pb.cpp

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lprotobuf

HEADERS += \
    request.pb.h

DISTFILES += \
    request.proto

