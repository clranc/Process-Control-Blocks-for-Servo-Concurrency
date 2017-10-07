/*
Files       : Main.c
Description : Main code for determining counting wave periods seen
Authors     : Chris Ranc and Rohini Jayachandre
*/

#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"
#include "Timer.h"
#include "PWM.h"
#include "recipe_interpreter.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "Give Red LED control input (Y = On, N = off):\r\n";

recipe_process proc1,proc2;

void time_check(void){
    if (TIM1_Has_Ended()){
        if (proc1.p_state == waiting && proc1.wait_cnt > 0){
            proc1.wait_cnt--;
        }
        if (proc2.p_state == waiting && proc2.wait_cnt > 0){
            proc2.wait_cnt--;
        }
    }
}

int main(void){
    System_Clock_Init(); // Switch System Clock = 80 MHz
    UART2_Init();
    TIM1_Init();
    PWM_Init();

    // Main Loop
    while (1){
        time_check();
        process(proc1);
        process(proc2);
//        USART_Read_String(buffer);
//        pulse = atoi((char *) buffer);
//
//        if (pulse < MIN_DUTY_CYCLE)
//            pulse = MIN_DUTY_CYCLE;
//        else if (pulse > MAX_DUTY_CYCLE)
//            pulse = MAX_DUTY_CYCLE;
//        PWM_CH_set(pulse, CHANNEL1);
    }
}
