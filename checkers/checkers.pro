QT += qml quick quickcontrols2

CONFIG += c++11 qmltypes qml_debug

SOURCES += main.cpp \
    model.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    model.h
