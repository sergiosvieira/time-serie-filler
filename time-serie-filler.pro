TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    cxxopts.hpp \
    date.h

unix:!macx: LIBS += -L$$PWD/fmt/build/ -lfmt

INCLUDEPATH += $$PWD/fmt/build
DEPENDPATH += $$PWD/fmt/build

unix:!macx: PRE_TARGETDEPS += $$PWD/fmt/build/libfmt.a
