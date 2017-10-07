/*
File        : Timer.c
Description : Routines for initalizing and using Timer1 for determining
              the period of square waves
Author      : Chris Ranc
*/
#include "Timer.h"
#include "stm32l476xx.h"
#include <stdint.h>

void TIM1_Init(void){

    // Timer Configuration:
    // Enable Peripheral Clock for Timer 1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // Set Prescaler to Make Timer Count Every 100uS
    // Clock Frequency / Prescaler => (80MHz/8000 = 10KHz)
    TIM1->PSC = CLOCK_PRESCALER;

    // Create Update
    TIM1->EGR = TIM_EGR_UG;

    // Set Auto reload register
    TIM1->ARR = COUNT_DOWN

    // Configure Timer for Downcounting and Auto reloading
    TIM1->CR1 |= TIM_CR1_DIR | TIM_CR1_ARPE;
    
}

// Get Currently Stored Time
unsigned int TIM1_Get_Time(void){
    return ((unsigned int)TIM1->CCR2);
}

// Starts Timer 1
void TIM1_Start(void){
    TIM1->CR1 |= TIM_CR1_CEN;
}

// Stops Timer 1
void TIM1_Stop(void){
    TIM1->CR1 &= ~(TIM_CR1_CEN);
}

// See If a Downcount has Finished
int TIM1_Timer_Finish(void){
    return (TIM_SR_UIF & TIM1->SR);
}
