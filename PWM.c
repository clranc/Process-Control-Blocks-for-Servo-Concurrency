#include "PWM.h"
#include "stm32l476xx.h"

#include <stdint.h>


//
void PWM_init(void){
    // Timer 2 Disabled Before Configurations are Set
    TIM2->CCER &= ~(TIM_CCER_CC2E | TIM_CCER_CC1E);

    // Timer GPIO Initialization:
    // Enable Peripheral Clock for GPIO Port A
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Set Pin PA0 and PA1 to Alternate Function Mode
    GPIOA->MODER &= ~(0x0000000F);
    GPIOA->MODER |= 0x0000000A;

    // Set Alternate Function to Channels 1 & 2 of Timer 2
    // for Port A Pins PA0 and PA1
    GPIOA->AFR[0] &= ~(0x000000FF);
    GPIOA->AFR[0] |= 0x00000011;

    // Enable Peripheral Clock for Timer 2
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    // Set Timer 2 Prescaler and Update it
    // To have Timer count every 1us
    TIM2->PSC = PRESCALER;
    TIM2->EGR = TIM_EGR_UG;

    // Set PWM Duty Cycle of 20ms 
    TIM2->ARR &= 0;
    TIM2->ARR |= DUTY_CYCLE;

    // Clear Capture/Compare Mode Register for Channels 1 & 2
    TIM2->CCMR1 &= 0;

    // Configure Channel 1 & 2 on Timer 2 to PWM Mode 1 and to 
    // Enable Preload Mode for their Capture Compare Registers
    TIM2->CCMR1 |= 0x00006868;

    // Preset Capture Compare Register
    TIM2->CCR1 = PRELOAD_PULSE_TIME;
    TIM2->CCR2 = PRELOAD_PULSE_TIME;
    
    TIM2->CR1 |= TIM_CR1_ARPE;
    // Enable Timer 2
    TIM2->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    
    // Start Timer2  and Enable Auto-reload Preload
    TIM2->CR1 |= TIM_CR1_CEN;

} 

void PWM_CH1_set(uint16_t pulse){
   TIM2->CCR1 = pulse;
}

void PWM_CH2_set(uint16_t pulse){
    TIM2->CCR2 = pulse;
   
}
