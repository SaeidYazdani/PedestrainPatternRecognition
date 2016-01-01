TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    trainer.cpp \
    manager.cpp

RESOURCES += qml.qrc

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_highgui -lopencv_imgcodecs -lopencv_core

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    trainer.h \
    manager.h \
    pedrecog_types.h
