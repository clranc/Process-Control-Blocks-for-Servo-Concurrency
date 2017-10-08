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

#include <stdio.h>
#include <stdlib.h>

#define USER_BUFFER_SIZE 3

uint8_t buffer[BufferSize];
uint8_t user_buffer[USER_BUFFER_SIZE];
int u_buff_pos = 0;

char str[] = "Give Red LED control input (Y = On, N = off):\r\n";

unsigned char recipe1 [] = WAIT_RECIPE;
unsigned char recipe2 [] = WAIT_RECIPE;

recipe_process proc1;
recipe_process proc2;

int valid_char(uint8_t input){
    switch (input) {
    case 'P' :
    case 'p' :
    case 'C' :
    case 'c' :
    case 'R' :
    case 'r' :
    case 'L' :
    case 'l' :
    case 'N' :
    case 'n' :
    case 'B' :
    case 'b' :
    case 'X' :
    case 'x' :
    case '\r' :
    case 0x7F :
        return 1;
    default :
        return 0;
    }
}

void time_check(void){
    if (TIM1_Has_Ended()) {
        if ((proc1.p_state == waiting || proc1.p_state == servo_running) &&
            proc1.s_state == not_paused && proc1.wait_cnt > 0){
            proc1.wait_cnt--;
        }
        if ((proc2.p_state == waiting || proc2.p_state == servo_running) &&
            proc2.s_state == not_paused && proc2.wait_cnt > 0){
            proc2.wait_cnt--;
        }
    }
}

void set_LED_servo1(void){
    switch (proc1.d_state) {
    case is_running:
        Green_LED_On();
        Red_LED_Off();
        break;
    case is_paused:
        Green_LED_Off();
        Red_LED_Off();
        break;
    case nested_loop_error:
        Green_LED_On();
        Red_LED_On();
        break;
    case command_error:
        Green_LED_Off();
        Red_LED_On();
        break;
    default:
        break;
    }
}

void USART2_IRQHandler(void){
    uint8_t input = (uint8_t)(USART2->RDR & 0xFF);

    if (valid_char(input)){
        if(input =='\r'){
            if (u_buff_pos == USER_BUFFER_SIZE - 1) {
                user_buffer[u_buff_pos] = '\0';
                USART_Write(USART2, (uint8_t *)"\r\n>", 3);
                if (user_buffer[0] != 'x' && user_buffer[0] != 'X' &&
                    user_buffer[1] != 'x' && user_buffer[1] != 'X'){
                    proc1.user_instr = user_buffer[0];
                    proc1.u_state = user_input;
                    proc2.user_instr = user_buffer[1];
                    proc2.u_state = user_input;
                }
                u_buff_pos = 0;
            }
        }

        // Check if the user entered a backspace to delete
        // an entered character and decrement the buffer
        // index
        else if (input==0x7F){
            if (u_buff_pos != 0){
                USART_Write(USART2, &input, 1);
                u_buff_pos--;
            }
        }

        // If the buffer limit hasn't been met append
        // the character to the string and increment
        // the string index
        else if( u_buff_pos <  USER_BUFFER_SIZE - 1){
            USART_Write(USART2, &input, 1);
            user_buffer[u_buff_pos] = input;
            u_buff_pos++;
        }
    }
}

void console_print(void){
    USART_Write(USART2, (uint8_t *) ">", 1);
}

int main(void){
    System_Clock_Init(); // Switch System Clock = 80 MHz
    UART2_Init();
    console_print();
    PWM_Init();
    TIM1_Init();
    LED_Init();
    TIM1_Start();
    init_process(&proc1, recipe1, CHANNEL1);
    init_process(&proc2, recipe2, CHANNEL2);

    // Main Loop
    while (1){
        set_LED_servo1();
        time_check();
        process(&proc1);
        process(&proc2);
    }
}
