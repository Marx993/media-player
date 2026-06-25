QT += widgets gui core

CONFIG += c++17

SOURCES += main.cpp

INCLUDEPATH += $$system(pkg-config --cflags libvlc)

LIBS += $$system(pkg-config --libs libvlc)
