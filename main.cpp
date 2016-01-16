#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QDate>

#include "manager.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("EMBEDONIX");
    app.setOrganizationDomain("EMBEDONIX.COM");
    app.setApplicationVersion("0.0.1 ALPHA");
    app.setApplicationName("Pedestrain Pattern Recognition");


    Manager manager;
    QQmlApplicationEngine engine;

    auto root_context = engine.rootContext();    
    root_context->setContextProperty("cpManager", &manager);


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
