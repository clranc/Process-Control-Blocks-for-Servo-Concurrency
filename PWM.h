#include <stdint.h>

#define PRESCALER 79
#define DUTY_CYCLE (uint16_t) 20000
#define PRELOAD_PULSE_TIME (uint16_t) 388

void PWM_init(void);
void PWM_CH1_set(uint16_t pulse);
void PWM_CH2_set(uint16_t pulse);
