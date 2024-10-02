#include <QGuiApplication>
#include <QQmlApplicationEngine>
# include "MusicPlayer.hpp"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/MusicMicroService/Main.qml"));
    qmlRegisterType<MusicPlayer>("com.MusicPlayer", 1, 0, "MusicPlayer");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
