#include <QGuiApplication>
#include <QQmlApplicationEngine>
# include "MusicPlayer.hpp"
# include <QIcon>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("play-button.svg"));
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
