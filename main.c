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
#include "LED.h"
#include "recipe_interpreter.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "Give Red LED control input (Y = On, N = off):\r\n";

//unsigned char recipe [] = {}

//recipe_process proc1,proc2;

//void time_check(void){
//    if (TIM1_Has_Ended()){
//        if ((proc1.p_state == waiting || proc1.p_state == servo_running) &&
//            proc1.wait_cnt > 0){
//            proc1.wait_cnt--;
//        }
//        if ((proc2.p_state == waiting || proc2.p_state == servo_running) &&
//            proc2.wait_cnt > 0){
//            proc2.wait_cnt--;
//        }
//    }
//}



int main(void){
    int cnt = 10;
    System_Clock_Init(); // Switch System Clock = 80 MHz
    //UART2_Init();
    TIM1_Init();
    LED_Init();
    Red_LED_On();
    TIM1_Start();
    //PWM_Init();
    //init_process(&proc1, &recipe, CHANNEL1);
    //init_process(&proc2, &recipe, CHANNEL2);
    // Main Loop
    while (1){
        if (TIM1_Has_Ended()) {
            if (cnt > 0 )
                cnt--;
            else{
                Red_LED_Toggle();
                Green_LED_Toggle();
                cnt = 10;
            }
        }
//        time_check();
//        process(proc1);
//        process(proc2);

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
