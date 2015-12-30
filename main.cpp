#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QDate>

#include "trainer.h"
#include "training_type.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    auto root_context = engine.rootContext();
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
