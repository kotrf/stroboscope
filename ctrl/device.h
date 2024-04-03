#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
//#include "log.h"

class Device : public QObject
{
    Q_OBJECT

    static constexpr quint8 CmdSetPwmFreq = '1';
    static constexpr quint8 CmdSetPwmDuty = '2';
    static constexpr quint8 CmdSetRelay   = '3';

    static constexpr quint32 PortBaudRate = 115200;
    static const QSerialPort::DataBits PortOptDataBits = QSerialPort::Data8;
    static const QSerialPort::Parity PortOptParity = QSerialPort::NoParity;
    static const QSerialPort::StopBits PortOptStopBits = QSerialPort::OneStop;
    static const QSerialPort::FlowControl PortOptFlowControl = QSerialPort::NoFlowControl;

    static const QByteArray Sync;

    QString m_portName;
    QSerialPort m_port;
    bool m_portIsOpen;

    void send(quint8 cmdCode, const QByteArray &data);

private slots:
//    void onReadyRead();

public:
    explicit Device(const QString &portName, QObject *parent = 0);
    ~Device();
    bool open();
    void close();
    void setPwmFreq(quint16 freq);
    void setPwmDuty(quint16 duty);
    void setRelay(bool on);
    bool isOpen() { return m_portIsOpen; }

signals:
    void logInfo(const QString &message, int debugLevel = 0);
    void logError(const QString &message, int debugLevel = 0);
};

#endif // DEVICE_H
