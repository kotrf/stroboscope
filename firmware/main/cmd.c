#include <stdint.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "sdkconfig.h"
#include "pwm.h"
#include "relay.h"

#define CMD_CODE_SET_PWM_FREQ   '1'
#define CMD_CODE_SET_PWM_DUTY   '2'
#define CMD_CODE_SET_RELAY      '3'

#define CMD_SYNC_SIZE       4
#define CMD_CODE_SIZE       1
#define CMD_PARAM_SIZE      2
#define CMD_RAW_SIZE        (CMD_SYNC_SIZE + CMD_CODE_SIZE + CMD_PARAM_SIZE)

static const uint8_t cmd_sync[CMD_SYNC_SIZE] = {'s', 'b', 'e', 'r'};

static uint8_t cmd_raw[CMD_RAW_SIZE];
static size_t cmd_raw_index = 0;
static size_t cmd_raw_tail = 0;
static size_t cmd_raw_head = 0;

typedef void (*CmdHandler)(uint8_t prm0, uint8_t prm1); 

struct CmdTableItem
{
    uint8_t cmd_code;
    CmdHandler handler;
};

void cmd_handler_set_pwm_freq(uint8_t prm0, uint8_t prm1);
void cmd_handler_set_pwm_duty(uint8_t prm0, uint8_t prm1);
void cmd_handler_set_relay(uint8_t prm0, uint8_t prm1);

static struct CmdTableItem cmd_table[] = {
    { CMD_CODE_SET_PWM_FREQ, cmd_handler_set_pwm_freq },
    { CMD_CODE_SET_PWM_DUTY, cmd_handler_set_pwm_duty },
    { CMD_CODE_SET_RELAY,    cmd_handler_set_relay    },
};

static const size_t cmd_table_len = sizeof(cmd_table) / sizeof(cmd_table[0]);

void cmd_handler_set_pwm_freq(uint8_t prm0, uint8_t prm1)
{
    uint freq = (((uint16_t)prm1) << 8) | ((uint16_t)prm0);
    pwm_set_freq(freq);
}

void cmd_handler_set_pwm_duty(uint8_t prm0, uint8_t prm1)
{
    uint duty = (((uint16_t)prm1) << 8) | ((uint16_t)prm0);
    pwm_set_duty(duty);
}

void cmd_handler_set_relay(uint8_t prm0, uint8_t prm1)
{
    relay_set((bool)prm0);
}

bool isSyncFound()
{
    size_t sync_index = cmd_raw_tail;

    for (int n=0; n<CMD_SYNC_SIZE; ++n)
    {
        if (cmd_sync[n] != cmd_raw[sync_index])
            return false;
        sync_index = (sync_index + 1) % CMD_RAW_SIZE;
    }

    return true;
}

void cmd_on_rx_data(uint8_t *data, size_t data_size)
{
    for (size_t n=0; n<data_size; ++n)
    {
        cmd_raw[cmd_raw_head] = data[n];
        cmd_raw_head = (cmd_raw_head + 1) % CMD_RAW_SIZE;

        if (++cmd_raw_index == CMD_RAW_SIZE)
        {
            if (!isSyncFound())
            {
                cmd_raw_tail = (cmd_raw_tail + 1) % CMD_RAW_SIZE;
                --cmd_raw_index;
            }
            else
            {
                uint8_t cmd_code = cmd_raw[(cmd_raw_tail+CMD_SYNC_SIZE) % CMD_RAW_SIZE];
                uint8_t cmd_prm0 = cmd_raw[(cmd_raw_tail+CMD_SYNC_SIZE+CMD_CODE_SIZE) % CMD_RAW_SIZE];
                uint8_t cmd_prm1 = cmd_raw[(cmd_raw_tail+CMD_SYNC_SIZE+CMD_CODE_SIZE+1) % CMD_RAW_SIZE];
                cmd_raw_index -= CMD_RAW_SIZE;
                
                for (int n=0; n<cmd_table_len; ++n)
                {
                    if (cmd_table[n].cmd_code == cmd_code)
                        cmd_table[n].handler(cmd_prm0, cmd_prm1);
                }
            }
        }
    }
}
