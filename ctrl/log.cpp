#include <QTime>
#include <QDate>
#include <QStringLiteral>
#include <iostream>
#include "log.h"

using namespace std;

Log::Log(QObject *parent)
    : QObject(parent)
    , m_debugLevel(0)
{
}

void Log::setLevel(int debugLevel)
{
    m_debugLevel = debugLevel;
}

void Log::info(const QString &message, int debugLevel)
{
    if ( m_debugLevel < debugLevel )
        return;

    cout << message.toStdString() << endl;
}

void Log::error(const QString &message, int debugLevel)
{
    if ( m_debugLevel < debugLevel )
        return;

    cerr << "ERROR! " << message.toStdString() << endl;
}
