#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QDate>

#include "manager.h"
#include "trainer.h"
#include "patrec_types.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Manager manager;

    auto root_context = engine.rootContext();    
    root_context->setContextProperty("cpManager", &manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
