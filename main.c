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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "Give Red LED control input (Y = On, N = off):\r\n";

void delay(unsigned int cnt){
    while(cnt){
      cnt--;
    }
}

// POST Routine 
void POST(void) {
    int len;
    TIM1_Start();

    // Delay so there is enough time for the timer to recieve an event
    delay(20000);
    do{
        if(TIM1_Has_Count()!= 0)
            break;
        else{
            len = sprintf((char*)buffer, "\r\nYou messed up bro.  Press any key to try the POST again.");
            USART_Write(USART2, buffer, len);
            USART_Read(USART2);
            USART_Write(USART2, (uint8_t *)0x7F, 1);
        }
    }while(1);

    len = sprintf((char*)buffer, "\r\nPost was successfull\r\n");
    USART_Write(USART2, buffer, len);
    TIM1_Stop();
}

int main(void){
    //char rxByte;
    int lower_bound,upper_bound;
    int length,b_index, index;
    uint16_t pulse;
    unsigned int b_value, old_value = 0, delta;
 
    int bucket[101];


    System_Clock_Init(); // Switch System Clock = 80 MHz
    UART2_Init();
    PWM_init();

    // Main Loop
    while (1){
        USART_Read_String(buffer);
        pulse = atoi((char *) buffer);

        if (pulse < MIN_DUTY_CYCLE)
            pulse = MIN_DUTY_CYCLE;
        else if (pulse > MAX_DUTY_CYCLE)
            pulse = MAX_DUTY_CYCLE;
        PWM_CH_set(pulse, CHANNEL1);
    }
}
