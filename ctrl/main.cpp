#include <QObject>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <iostream>
#include "config.h"
#include "mainobject.h"

static const QString AppName = QStringLiteral("stroboscope");
static const uint verMajor = 0;
static const uint verMinor = 1;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(AppName);
    QCoreApplication::setApplicationVersion(
        QObject::tr("%1.%2")
            .arg(verMajor)
            .arg(verMinor));
    QCoreApplication::setOrganizationDomain("sber");

    auto conf = new Config(app);
    if ( !conf->cmdLine() )
    {
        std::cerr << "Error: " << conf->errorMessage().toStdString() << std::endl;
        return 2;
    }

    MainObject mainObject(conf->params(), verMajor, verMinor);
    QObject::connect(&mainObject, &MainObject::quit, &app, &QCoreApplication::quit);

    return app.exec();
}
