/* 
 * File        : Timer.h
 * Description : Contains Function Prototypes for configuring and 
 *               utilizing Timer 1 for downcounting
 * Author      : Chris Ranc
 *
 */

#include <stdint.h>
void TIM1_Init(void);
void TIM1_Start(void);
void TIM1_Stop(void);
int TIM1_Has_Ended(void);

#define POST_CLOCK_PRESCALER (uint16_t) 799
#define CLOCK_PRESCALER (uint16_t) 7999
#define COUNT_DOWN (uint16_t) 1000
#define CLOCK_FREQ 80000000
