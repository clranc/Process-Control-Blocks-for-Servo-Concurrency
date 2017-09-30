/*
File        : Timer.c
Description : Routines for initalizing and using Timer1 for determining
               the period of square waves
Author      : Chris Ranc
*/
#include "Timer.h"
#include "stm32l476xx.h"
#include <stdint.h>

// Initializes Timer8 to Produce PWM 
//
void TIM1_Init(uint16_t prescaler){
    // Making sure timer capture and control is
    // disabled before configurations are set
    TIM1->CCER &= ~(0xFFFFFFFF);

    // Timer GPIO Initialization:
    // Enable Peripheral Clock for GPIO Port E
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

    // Set Pin PE11 to Alternate Function Mode
    GPIOE->MODER &= ~(0x00C00000);
    GPIOE->MODER |= 0x00800000;

    // Set Alternate Function to Timer 1 Channel 2
    GPIOE->AFR[1] &= ~(0x0000F000);
    GPIOE->AFR[1] |= 0x00001000;

    // Timer Configuration:
    // Enable Peripheral Clock for Timer 1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // Set Prescaler to Make Timer Count Every 1uS
    // Clock Frequency / Prescaler => (80MHz/80 = 1MHz)
    TIM1->PSC = prescaler;

    // Create Update
    TIM1->EGR = TIM_EGR_UG;

    // Configure Channel as Input
    TIM1->CCMR1 = (TIM_CCMR1_CC2S_0 | TIM_CCMR1_OC2CE);

    // Enable Timer
    TIM1->CCER = TIM_CCER_CC2E;
}

// Get Currently Stored Time
unsigned int TIM1_Get_Time(void){
    return ((unsigned int)TIM1->CCR2);
}

// Starts Timer 1
void TIM1_Start(void){
    TIM1->CCR2 = 0x0000;
    TIM1->CR1 |= 0x1;
}

// Stops Timer 1
void TIM1_Stop(void){
    TIM1->CR1 &= ~(0x1);
}

// See If a Count Capture Has Occurred 
int TIM1_Has_Count(void){
    return (0x4 & TIM1->SR);
}
