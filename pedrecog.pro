TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    trainer.cpp \
    manager.cpp \
    arffgenerator.cpp \
    helpers.cpp \
    bayesianclassifier.cpp \
    statisticscalculator.cpp \
    negativefoldersparser.cpp

RESOURCES += qml.qrc

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui \
    -lopencv_imgproc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    trainer.h \
    manager.h \
    arffgenerator.h \
    patrec_types.h \
    bayesianclassifier.h \
    statisticscalculator.h \
    negativefoldersparser.h
