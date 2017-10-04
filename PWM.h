#include <stdint.h>

#define PRESCALER 79
#define CYCLE_PERIOD (uint16_t) 20000
#define MIN_DUTY_CYCLE (uint16_t) 388
#define MAX_DUTY_CYCLE (uint16_t) 2000

void PWM_init(void);
void PWM_CH1_set(uint16_t pulse);
void PWM_CH2_set(uint16_t pulse);
