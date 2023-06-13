#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <model.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MyModel *model = new MyModel(&app);
    engine.rootContext()->setContextProperty("Model", model);
    //qmlRegisterType<MyModel>("MyModel", 1, 0, "MyModel");

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
