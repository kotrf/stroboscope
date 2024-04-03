#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QList>

class Config : public QObject
{
    Q_OBJECT

    static constexpr quint16 FreqMax = 1000;
    static const QStringList BoolTrueValues;
    static const QStringList BoolFalseValues;

    static const QString ArgFreq;
    static const QString ArgPulseWidth;
    static const QString ArgRelay;

    static const QList<QCommandLineOption> CmdLineOptions;

public:
    struct Param {
        QString name;
        QString value;
        bool present;
    };

    static const QStringList ParamNames;

    struct Params {
        bool setPwmFreqHz;
        bool setPwmPulseWidthUs;
        bool setRelayOn;
        quint16 pwmFreqHz;
        quint16 pwmPulseWidthUs;
        bool relayOn;
        QString serialPortName;
    };

    const QCoreApplication &m_app;
    Params m_params;
    //    bool m_initDone;
    QString m_errorMessage;

    bool parseBoolValue(const QString &str, bool &value);

public:
    explicit Config(QCoreApplication &app, QObject *parent = nullptr);
    const Params &params();
    const QString &errorMessage();
    bool cmdLine();
};

#endif // CONFIG_H
