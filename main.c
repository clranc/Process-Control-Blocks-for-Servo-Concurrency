/*
 * Files       : Main.c
 * Description : Main code for performing concurrent operation of servos while
 *               handling user control input
 * Authors     : Chris Ranc
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

unsigned char recipe1 [] = DEMO_RECIPE;
unsigned char recipe2 [] = BAD_INSTR_RECIPE;

recipe_process proc1;
recipe_process proc2;

// Functions Utilized by Main to Operate
// Full Descriptions can be seen below main
int valid_char(uint8_t input);
void time_check(void);
void set_LED_servo1(void);
void console_print(void);

/* Main Routine */
int main(void){
    /* Hardware and State Initialization */
    System_Clock_Init(); /* Switch System Clock = 80 MHz */
    UART2_Init();
    console_print();
    PWM_Init();
    TIM1_Init();
    LED_Init();
    TIM1_Start();
    init_process(&proc1, recipe1, CHANNEL1);
    init_process(&proc2, recipe2, CHANNEL2);

    /* Main Loop */
    while (1){
        set_LED_servo1();
        time_check();
        process(&proc1);
        process(&proc2);
    }
}

/* UART interrupt request handler to process user input.  If an accepted
 * character has been entered then it will be stored in the user
 * command buffer or be used to determine command entry/deletion.
 */
void USART2_IRQHandler(void){
    uint8_t input = (uint8_t)(USART2->RDR & 0xFF);

    if (valid_char(input)){
        /* Check if the user sent a carriage return to attempt submitting
         * the entered user commands.  
         */
        if(input =='\r'){
            /* If 2 have been loaded then they
             * will be checked to see if cancel characters x/X have been
             * entered to cancel user entered commands and reset the buffer and
             * command line for next entry.
             */
            if (u_buff_pos == USER_BUFFER_SIZE - 1) {
                user_buffer[u_buff_pos] = '\0';
                USART_Write(USART2, (uint8_t *)"\r\n>", 3);
                /* If no cancel characters have been
                 * entered then they are stored in the process control block
                 * along with changing its state to process the the user input.
                 */
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

        /* Check if the user entered a backspace to delete
         * an entered character and decrement the buffer
         * index
         */
        else if (input==0x7F){
            if (u_buff_pos != 0){
                USART_Write(USART2, &input, 1);
                u_buff_pos--;
            }
        }

        /* If the buffer limit hasn't been met append
         * the character to the string and increment
         * the string index
         */
        else if( u_buff_pos <  USER_BUFFER_SIZE - 1){
            USART_Write(USART2, &input, 1);
            user_buffer[u_buff_pos] = input;
            u_buff_pos++;
        }
    }
}

/* Sees if the entered user character is appropriate
 * as a command or deleting/entering the commands
 */
int valid_char(uint8_t input){
    switch (input) {
    case 'P' : /* Pause */
    case 'p' :
    case 'C' : /* Continue */
    case 'c' :
    case 'R' : /* Move Right */
    case 'r' :
    case 'L' : /* Move Left */
    case 'l' :
    case 'N' : /* No Operation */
    case 'n' :
    case 'B' : /* Reset */
    case 'b' :
    case 'X' : /* Cancel Command Entry */
    case 'x' :
    case '\r' : /* Attempt Command Entry */
    case 0x7F : /* Backspace for deleting entered Character */
        return 1;
    default :
        return 0;
    }
}

/* Check if 100ms Have Passed and Check Recipe Process Block States to see if they
 * are in a Waiting or Servo Running State to Decrement Wait Time that has not
 * Passed
 */
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

/* Set LEDs for the Recipe Process Control Block Controlling the First Servo
 * on Channel 1
 */
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

/* Print Console Start Character */
void console_print(void){
    USART_Write(USART2, (uint8_t *) ">", 1);
}
