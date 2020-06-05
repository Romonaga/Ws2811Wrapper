#-------------------------------------------------
#
# Project created by QtCreator 2017-06-20T20:36:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = Ws2811Wrapper
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11

SOURCES += ws2811wrapper.cpp 

HEADERS += ws2811wrapper.h

unix {
    target.path = /usr/local/lib
    INSTALLS += target
}

unix {
    headers.path = /usr/local/include
    headers.files = *.h
    INSTALLS += headers
}





