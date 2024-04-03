#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QStringList>

class Log : public QObject
{
    Q_OBJECT

    int m_debugLevel;
    const QString dateTimeString();

public:
    explicit Log(QObject *parent = nullptr);
    void setLevel(int debugLevel);

public slots:
    void info(const QString &message, int debugLevel = 0);
    void error(const QString &message, int debugLevel = 0);
};

#endif // LOG_H
