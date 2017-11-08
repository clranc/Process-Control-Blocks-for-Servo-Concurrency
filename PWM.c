/*
 * File        : PWM.c
 * Description : Routines for initializing and using Timer 2 for generating
 *               output PWM signals on PORT A pins 0 and 1 
 * Author      : Chris Ranc
 */
#include "PWM.h"
#include "stm32l476xx.h"

#include <stdint.h>


void PWM_Delay(uint32_t time) {
    //uint32_t time = 100*us/7;
    while(--time);
}

/* Initialization sub-routine to configure Timer 2
 * and Port A Pins 0 and 1 for PWM
 */
void PWM_Init(void){
    /* Timer 2 disabled before configurations are set */
    TIM2->CCER &= ~(TIM_CCER_CC2E | TIM_CCER_CC1E);

    /* Timer GPIO Initialization:
     * Enable peripheral clock for GPIO Port A
     */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* Set Pin PA0 and PA1 to Alternate Function Mode */
    GPIOA->MODER &= ~(0x0000000F);
    GPIOA->MODER |= 0x0000000A;

    /* Set Alternate Function to Channels 1 & 2 of Timer 2
     * for Port A Pins PA0 and PA1
     */
    GPIOA->AFR[0] &= ~(0x000000FF);
    GPIOA->AFR[0] |= 0x00000011;

    /* Enable Peripheral Clock for Timer 2 */
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    /* Set Timer 2 Prescaler and update it
     * To have Timer count every 1us 
     */
    TIM2->PSC = PRESCALER;
    TIM2->EGR = TIM_EGR_UG;

    /* Set PWM Period of 20ms */
    TIM2->ARR &= 0;
    TIM2->ARR |= CYCLE_PERIOD;

    /* Clear Capture/Compare Mode Register for Channels 1 & 2 */
    TIM2->CCMR1 &= 0;

    /* Configure Channel 1 & 2 on Timer 2 to PWM Mode 1 and to
     * Enable Preload Mode for their Capture Compare Registers
     */
    TIM2->CCMR1 |= 0x00006868;

    /* Preset Capture Compare Register */
    TIM2->CCR1 = MIN_DUTY_CYCLE;
    TIM2->CCR2 = MIN_DUTY_CYCLE;

    /* Enable Auto-reload Preload */
    TIM2->CR1 |= TIM_CR1_ARPE;

    /* Enable Timer 2 */
    TIM2->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;

    /* Start Timer2  */
    TIM2->CR1 |= TIM_CR1_CEN;
}

/* Set the Duty Cycle of One of the Configured Output Channels */
void PWM_CH_Set(uint32_t pulse, enum pwm_ch ch){
    switch (ch) {
        case CHANNEL1 :
            TIM2->CCR1 = pulse;
            break;
        case CHANNEL2 :
            TIM2->CCR2 = pulse;
        default:
            break;
    }
}
