/* 
File        : Timer.h
Description : Contains function Prototypes for configuring and 
              utilizing Timer 1 to determine the period of 
              square waves
Author      : Chris Ranc
*/
#include <stdint.h>
void TIM1_Init(uint16_t prescaler);
unsigned int TIM1_Get_Time(void);
void TIM1_Start(void);
void TIM1_Stop(void);
int TIM1_Has_Count(void);

#define POST_CLOCK_PRESCALER (uint16_t) 799
#define CLOCK_PRESCALER (uint16_t) 7999
#define COUNT_DOWN (uint16_t) 1000
#define CLOCK_FREQ 80000000
