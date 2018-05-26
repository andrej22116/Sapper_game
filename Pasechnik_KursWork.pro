TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

LIBS += -lGL -lGLU
LIBS += -lglut

HEADERS += \
    globalvariables.h \
    datastructures.h \
    stb_image.h


CONFIG += precompile_header
PRECOMPILED_HEADER = stb_image.h
