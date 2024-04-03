#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include <QObject>
#include <log.h>
#include <device.h>
#include "config.h"

class MainObject : public QObject
{
    Q_OBJECT

    const Config::Params &m_conf;
    Log *log;
    Device *device;

private slots:
    void task();

public:
    explicit MainObject(const Config::Params &conf, uint verMajor, uint verMinor, QObject *parent = nullptr);
    ~MainObject();

signals:
    void quit();
};

#endif // MAINOBJECT_H
