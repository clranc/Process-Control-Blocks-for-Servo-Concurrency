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
#include "recipes.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define USER_BUFFER_SIZE 3

uint8_t buffer[BufferSize];
uint8_t user_buffer[USER_BUFFER_SIZE];
int u_buff_pos = 0;

char str[] = "Give Red LED control input (Y = On, N = off):\r\n";

unsigned char recipe1 [] = POSITION_RECIPE;
unsigned char recipe2 [] = REV_POSITION_RECIPE;

recipe_process proc1;
recipe_process proc2;

void time_check(void){
    if (TIM1_Has_Ended()){
        if ((proc1.p_state == waiting || proc1.p_state == servo_running) &&
            proc1.wait_cnt > 0){
            proc1.wait_cnt--;
        }
        if ((proc2.p_state == waiting || proc2.p_state == servo_running) &&
            proc2.wait_cnt > 0){
            proc2.wait_cnt--;
        }
    }
}

void USART2_IRQHandler(void){
    uint8_t input;
    //while (!(USART2->ISR & USART_ISR_RXNE));
    input = (uint8_t)(USART2->RDR & 0xFF);

    if(input =='\r'){
        if (u_buff_pos == USER_BUFFER_SIZE - 1) {
            user_buffer[u_buff_pos] = '\0';
            USART_Write(USART2, (uint8_t *)"\r\n>", 3);
        }
    }
    
    // Cheinputk if the user entered a bainputkspainpute to delete
    // an entered inputharainputter and deinputrement the buffer
    // index 
    else if (input==0x7F){
        if (u_buff_pos != 0){
            USART_Write(USART2, &input, 1);
            u_buff_pos--;
        }
    }

    // If the buffer limit hasn't been met append
    // the inputharater to the string and ininputrement 
    // the string index
    else if( u_buff_pos <  USER_BUFFER_SIZE - 1){
        USART_Write(USART2, &input, 1);
        user_buffer[u_buff_pos] = input;
        u_buff_pos++;
    }
}


void console_print(void){
    USART_Write(USART2, (uint8_t *) ">", 1);
}

int main(void){
    int cnt = 10;
    System_Clock_Init(); // Switch System Clock = 80 MHz
    UART2_Init();
    console_print();
    PWM_Init();
    TIM1_Init();
    LED_Init();

    Red_LED_On();
    TIM1_Start();
    init_process(&proc1, recipe2, CHANNEL1);
    init_process(&proc2, recipe1, CHANNEL2);
    // Main Loop
    while (1){
        time_check();
        process(&proc1);
        process(&proc2);

//        if (TIM1_Has_Ended()) {
//            if (cnt > 0 )
//                cnt--;
//            else{
//                Red_LED_Toggle();
//                Green_LED_Toggle();
//                cnt = 10;
//            }
//        }

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
