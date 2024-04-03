#include <QTest>
#include "device.h"

// add necessary includes here

class Ctrl : public QObject
{
    Q_OBJECT

public:
    Ctrl();
    ~Ctrl();

private slots:
    void test_openPort();
};

Ctrl::Ctrl()
{
//    QVERIFY(false);
}

Ctrl::~Ctrl()
{

}

void Ctrl::test_openPort()
{
    auto *device = new Device(QStringLiteral("/dev/ttyACM0"));
    bool isOpen = device->open();
    QVERIFY(isOpen);
}

QTEST_MAIN(Ctrl)

#include "tst_Ctrl.moc"
