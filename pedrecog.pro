TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    trainer.cpp \
    manager.cpp \
    arffgenerator.cpp \
    bayesianclassifier.cpp \
    statisticscalculator.cpp \
    negativefoldersparser.cpp \
    pr_helper.cpp

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
    bayesianclassifier.h \
    statisticscalculator.h \
    negativefoldersparser.h \
    pr_helper.h
