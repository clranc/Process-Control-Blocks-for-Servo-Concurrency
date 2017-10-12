/*
 * File        : Timer.c
 * Description : Routines for initalizing and using Timer1 for
 *               downcounting 
 * Author      : Chris Ranc
 */
#include "Timer.h"
#include "stm32l476xx.h"
#include <stdint.h>


// Initializes Timer 1 for Downcounting
void TIM1_Init(void){
    // Enable Peripheral Clock for Timer 1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // Set Prescaler to Make Timer Count Every 100uS
    // Clock Frequency / Prescaler => (80MHz/8000 = 10KHz)
    TIM1->PSC = CLOCK_PRESCALER;

    // Create Update
    TIM1->EGR = TIM_EGR_UG;

    // Set Auto reload register
    TIM1->ARR = COUNT_DOWN;

    // Configure Timer for Downcounting and Auto Reloading
    TIM1->CR1 |= TIM_CR1_DIR | TIM_CR1_ARPE;

}

// Starts Timer 1
void TIM1_Start(void){
    TIM1->CR1 |= TIM_CR1_CEN;
}

// Stops Timer 1
void TIM1_Stop(void){
    TIM1->CR1 &= ~(TIM_CR1_CEN);
}

// See if a Downcount has Finished
int TIM1_Has_Ended(void){
    if (TIM_SR_UIF & TIM1->SR){
        TIM1->SR &= ~(TIM_SR_UIF);
        return 1;
    }else {
        return 0;
    }
}
