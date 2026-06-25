QT += widgets gui core

CONFIG += c++17

SOURCES += main.cpp

CONFIG += link_pkgconfig
PKGCONFIG += libvlc

target.path = /usr/local/bin
INSTALLS += target
