#include <iostream>
#include <QTimer>
#include "mainobject.h"

MainObject::MainObject(const Config::Params &conf, uint verMajor, uint verMinor, QObject *parent)
    : QObject(parent)
    , m_conf(conf)
    , log(new Log)
    , device(new Device(conf.serialPortName))
{
    std::cout << tr("Stroboscope control v%1.%2")
                     .arg(verMajor)
                     .arg(verMinor)
                     .toStdString() << std::endl;

    connect(device, &Device::logInfo,  log, &Log::info);
    connect(device, &Device::logError, log, &Log::error);

    if ( !device->open() )
    {
        log->error(QStringLiteral("Unable open port"));
        QTimer::singleShot(0, this, [this] { emit quit(); });
        return;
    }

    QTimer::singleShot(0, this, &MainObject::task);
}

void MainObject::task()
{
    static constexpr double DutyMax = 16383.0;

    if (m_conf.setPwmFreqHz)
    {
        if (m_conf.pwmFreqHz > 0)
        {
            double T = 1000000.0 / double(m_conf.pwmFreqHz);
            double dutyF = m_conf.setPwmPulseWidthUs
                ? double(m_conf.pwmPulseWidthUs) / T * DutyMax
                : DutyMax / 2;
            if (dutyF > DutyMax)
                dutyF = DutyMax;
            quint16 duty = quint16(dutyF + 0.5);
            uint pulseWidth = uint(double(duty) / DutyMax * T);

            log->info(tr("Freq: %1 Hz  T: %2 us  PulseWidth: %3 us  Duty: %4%")
                          .arg(m_conf.pwmFreqHz)
                          .arg(uint(T + 0.5))
                          .arg(pulseWidth)
                          .arg(uint(double(duty)/DutyMax * 100 + 0.5)));
            device->setPwmFreq(m_conf.pwmFreqHz);
            device->setPwmDuty(duty);
        }
        else
        {
            bool on = m_conf.pwmPulseWidthUs > 0;
            log->info(QStringLiteral("No PWM  LED is ") +
                      (on ? QStringLiteral("ON") : QStringLiteral("OFF")));
            device->setPwmFreq(0);
            device->setPwmDuty(on ? 1 : 0);
        }
    }

    if (m_conf.setRelayOn)
    {
        log->info(tr("Relay: %1").arg(m_conf.relayOn ? QStringLiteral("on") :  QStringLiteral("off")));
        device->setRelay(m_conf.relayOn);
    }

    QTimer::singleShot(0, this, [this] { emit quit(); });
}

MainObject::~MainObject()
{
    delete device;
    delete log;
}
