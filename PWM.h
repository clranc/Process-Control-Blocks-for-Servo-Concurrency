/*
 * File        : PWM.h
 * Description : Contains function prototypes and defines for configuring and 
 *               utilizing Timer 2 for generating output PWM signals on PORT A
 *               pins 0 and 1
 * Author      : Chris Ranc
 */

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#define PRESCALER 79
#define CYCLE_PERIOD (uint16_t) 20000
#define MIN_DUTY_CYCLE (uint16_t) 400
#define MAX_DUTY_CYCLE (uint16_t) 2000

// Enumerators for specifing channels on the timer configured for PWM
enum pwm_ch{
    CHANNEL1,
    CHANNEL2
};

#endif /* PWM_H */

void PWM_Init(void);
void PWM_CH_Set(uint32_t pulse, enum pwm_ch ch);
