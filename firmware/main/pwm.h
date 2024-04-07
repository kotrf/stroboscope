#ifndef _PWM_H
#define _PWM_H

#include <stdint.h>

void pwm_init(void);
void pwm_set_freq(uint16_t freq);
void pwm_set_duty(uint16_t duty);


#endif // _PWM_H
