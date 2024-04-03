#include <QSettings>
#include <QDebug>
#include "config.h"

const QStringList Config::ParamNames = {
    { QStringLiteral("freq")    },  // 0
    { QStringLiteral("duty")    },  // 1
    { QStringLiteral("relay")   },  // 2
};

const QStringList Config::BoolTrueValues = {
    QStringLiteral("true"),
    QStringLiteral("on"),
    QStringLiteral("1"),
};

const QStringList Config::BoolFalseValues = {
    QStringLiteral("false"),
    QStringLiteral("off"),
    QStringLiteral("0"),
};

const QString Config::ArgFreq        = QStringLiteral("freq");
const QString Config::ArgPulseWidth  = QStringLiteral("width");
const QString Config::ArgRelay       = QStringLiteral("relay");

const QList<QCommandLineOption> Config::CmdLineOptions =
    {
        { {"f", Config::ArgFreq         },  "Set pulse frequency to <freq> in Hz",  "freq"  },
        { {"w", Config::ArgPulseWidth   },  "Set pulse width to <width> in us",     "width" },
        { {"r", Config::ArgRelay        },  "Set relay <on> or <off>",              "on|off" },
        };

Config::Config(QCoreApplication &app, QObject *parent)
    : QObject(parent)
    , m_app(app)
    , m_params{}
{
    m_errorMessage = QStringLiteral("No errors");
}

const Config::Params &Config::params()
{
    return m_params;
}

const QString &Config::errorMessage()
{
    return m_errorMessage;
}

bool Config::cmdLine()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Stroboscope control tool");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("serial-port", QStringLiteral("serial port device"));
    parser.addOptions(CmdLineOptions);

    // Process the actual command line arguments given by the user

    parser.process(m_app);

    // Positional arguments: serial-port
    const QStringList posArgs = parser.positionalArguments();
    if ( posArgs.size() < 1 )
    {
        m_errorMessage = QStringLiteral("Serial port must be specified\n");
        return false;
    }

    if ( posArgs.size() > 1 )
    {
        m_errorMessage = QStringLiteral("Only one serial port must be specified\n");
        return false;
    }

    m_params.serialPortName = posArgs.at(0);

    // -f
    if ( parser.isSet("f") )
    {
        bool valid;
        uint pwmFreqHz = parser.value("f").toUInt(&valid);
        if ( !valid || pwmFreqHz > FreqMax )
        {
            m_errorMessage = QStringLiteral("Invalid value after -f\n");
            return false;
        }

        m_params.pwmFreqHz = pwmFreqHz;
        m_params.setPwmFreqHz = true;
    }

    // -w
    if ( parser.isSet("w") )
    {
        bool valid;
        uint pwmPulseWidthUs = parser.value("w").toUInt(&valid);
        if ( !valid || pwmPulseWidthUs > std::numeric_limits<quint16>::max() )
        {
            m_errorMessage = QStringLiteral("Invalid value after -w\n");
            return false;
        }

        m_params.pwmPulseWidthUs = pwmPulseWidthUs;
        m_params.setPwmPulseWidthUs = true;
    }

    // -r
    if ( parser.isSet("r") )
    {
        bool relayOn;
        if ( !parseBoolValue(parser.value("r"), relayOn) )
        {
            m_errorMessage = QStringLiteral("Invalid value after -r\n");
            return false;
        }

        m_params.relayOn = relayOn;
        m_params.setRelayOn = true;
    }

    if (!m_params.setRelayOn && !m_params.setPwmFreqHz)
    {
        m_errorMessage = QStringLiteral("PWM frequency must be specified\n");
        return false;
    }

    return true;
}

bool Config::parseBoolValue(const QString &str, bool &value)
{
    bool valueIsValid = false;

    for ( auto const &item: BoolTrueValues )
    {
        if ( 0 == str.compare(item, Qt::CaseInsensitive) )
        {
            value = true;
            valueIsValid = true;
            break;
        }
    }

    if ( valueIsValid )
        return true;

    for ( auto const &item: BoolFalseValues )
    {
        if ( 0 == str.compare(item, Qt::CaseInsensitive) )
        {
            value = false;
            valueIsValid = true;
            break;
        }
    }

    if ( valueIsValid )
        return true;

    bool isNumber = false;
    uint number = str.toUInt(&isNumber);

    if ( isNumber )
    {
        if ( 1 == number )
        {
            value = true;
            valueIsValid = true;
        }
        else if ( 0 == number )
        {
            value = false;
            valueIsValid = true;
        }
    }

    return valueIsValid;
}
