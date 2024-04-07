#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

static uint16_t testFreq = 0;
static uint16_t testDuty = 0;
static uint16_t testRelayEn = false;

// Redefine handlers with extern "C"
extern "C" void cmd_handler_set_pwm_freq(uint8_t prm0, uint8_t prm1);
extern "C" void cmd_handler_set_pwm_duty(uint8_t prm0, uint8_t prm1);
extern "C" void cmd_handler_set_relay(uint8_t prm0, uint8_t prm1);

// Define own setters
extern "C" void pwm_set_freq(uint16_t freq)
{
    testFreq = freq;
}

extern "C" void pwm_set_duty(uint16_t duty)
{
    testDuty = duty;
}

extern "C" void relay_set(bool en)
{
    testRelayEn = en;
}

extern "C" void cmd_on_rx_data(uint8_t *data, size_t data_size);

TEST(main, setFreq1)
{
    uint8_t buffer[8] = {'s','b','e','r','1',1,0};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testFreq, 1);
}

TEST(main, setFreq256)
{
    uint8_t buffer[8] = {'s','b','e','r','1',0,1};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testFreq, 256);
}

TEST(main, setFreq0)
{
    uint8_t buffer[8] = {'s','b','e','r','1',0,0};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testFreq, 0);
}

TEST(main, setFreq1000)
{
    uint8_t buffer[8] = {'s','b','e','r','1',0xe8,0x03};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testFreq, 1000);
}

TEST(main, setDuty1)
{
    uint8_t buffer[8] = {'s','b','e','r','2',1,0};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testDuty, 1);
}

TEST(main, setDuty256)
{
    uint8_t buffer[8] = {'s','b','e','r','2',0,1};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testDuty, 256);
}

TEST(main, setDuty0)
{
    uint8_t buffer[8] = {'s','b','e','r','2',0,0};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testDuty, 0);
}

TEST(main, setDuty1500)
{
    uint8_t buffer[8] = {'s','b','e','r','2',0xdc,0x05};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testDuty, 1500);
}

TEST(main, setRelayOn)
{
    uint8_t buffer[8] = {'s','b','e','r','3',1,0};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testRelayEn, true);
}

TEST(main, setRelayOff)
{
    uint8_t buffer[8] = {'s','b','e','r','3',0,0};
    cmd_on_rx_data(buffer, 7);

    EXPECT_EQ(testRelayEn, false);
}

TEST(main, setFreq1001OneByte)
{
    testFreq = 0xFFFF;

    uint8_t buffer[1] = {'s'};
    cmd_on_rx_data(buffer, 1);
    buffer[0] = 'b';
    cmd_on_rx_data(buffer, 1);
    buffer[0] = 'e';
    cmd_on_rx_data(buffer, 1);
    buffer[0] = 'r';
    cmd_on_rx_data(buffer, 1);

    buffer[0] = '1';
    cmd_on_rx_data(buffer, 1);
    buffer[0] = 0xe9;
    cmd_on_rx_data(buffer, 1);
    buffer[0] = 0x03;
    cmd_on_rx_data(buffer, 1);

    EXPECT_EQ(testFreq, 1001);
}
