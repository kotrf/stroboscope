#include <QTimer>
#include <QtEndian>
#include "device.h"
#include <QtEndian>
#include <QDebug>

const QByteArray Device::Sync("sber", 4);

Device::Device(const QString &portName, QObject *parent)
    : QObject(parent)
    , m_portName(portName)
{
    if ( !portName.length() )
        return;

//    connect(&m_port, &QDevicePort::readyRead, this, &Device::onReadyRead);
//    QTimer::singleShot(0, this, [this] {onReadyRead(); });
}

Device::~Device()
{
    close();
}

bool Device::open()
{
    m_port.setPortName(m_portName);

    if ( m_port.open(QIODevice::ReadWrite) )
    {
        m_port.clear();
        m_portIsOpen =
            m_port.setBaudRate(PortBaudRate) &&
            m_port.setDataBits(PortOptDataBits) &&
            m_port.setParity(PortOptParity) &&
            m_port.setStopBits(PortOptStopBits) &&
            m_port.setFlowControl(PortOptFlowControl);
        if ( !m_portIsOpen )
            m_port.close();
        //        port.setReadBufferSize(256);
    }

    if ( m_portIsOpen )
        emit logInfo(tr("Port %1 open").arg(m_portName), 1);
    else
        emit logError(tr("Unable open port %1").arg(m_portName));

    return m_portIsOpen;
}

void Device::close()
{
    m_port.close();
    m_portIsOpen = false;
}

/**
 * @brief  Send data to device
 *
 * @param  cmdCode Command code
 * @param  data Raw data of params to command
 */
void Device::send(quint8 cmdCode, const QByteArray &data)
{
    if ( !m_portIsOpen )
        return;

    QByteArray sendBuffer(Sync);
    sendBuffer.append(char(cmdCode));
    sendBuffer.append(data);
    m_port.write(sendBuffer);
}

/**
 * @brief  Send command to set PWM freq
 *
 * @param  freq Frequecny of PWM signal
 */
void Device::setPwmFreq(quint16 freq)
{
    QByteArray buffer(2, char(0));
    qToLittleEndian(freq, buffer.data());
    send(CmdSetPwmFreq, buffer);
}

/**
 * @brief  Send command to set PWM duty
 *
 * @param  duty Pulse width in 1/16383 steps
 */
void Device::setPwmDuty(quint16 duty)
{
    QByteArray buffer(2, char(0));
    qToLittleEndian(duty, buffer.data());
    send(CmdSetPwmDuty, buffer);
}

/**
 * @brief  Send command to set relay ON or OFFs
 *
 * @param  on If true set relay to ON, set relay OFF otherwise
 */
void Device::setRelay(bool on)
{
    QByteArray buffer(2, char(0));
    if (on)
        buffer[0] = char(1);
    send(CmdSetRelay, buffer);
}
