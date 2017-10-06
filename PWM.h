#include <stdint.h>

#define PRESCALER 79
#define CYCLE_PERIOD (uint16_t) 20000
#define MIN_DUTY_CYCLE (uint16_t) 400
#define MAX_DUTY_CYCLE (uint16_t) 2000

enum pwm_ch{
    CHANNEL1,
    CHANNEL2
};

void PWM_init(void);
void PWM_CH_Set(uint32_t pulse, enum pwm_ch ch);
