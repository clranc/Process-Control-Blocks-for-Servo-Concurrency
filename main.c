/*
Files       : Main.c
Description : Main code for determining counting wave periods seen
Authors     : Chris Ranc and Rohini Jayachandre
*/


#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"
#include "Timer.h"

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
    uint8_t select;
    unsigned int b_value, old_value = 0, delta;

    int bucket[101];


    System_Clock_Init(); // Switch System Clock = 80 MHz
    UART2_Init();
    TIM1_Init(CLOCK_PRESCALER);
    POST();
    TIM1_Start();

    // Main Loop
    while (1){
        lower_bound = 950;

        // User Intro Menu
        length = sprintf((char*)buffer, "Would you like to use a custom lowerbound period?\r\nEnter y if yes or any other key to use the preset of %uus\r\n",lower_bound);
        USART_Write(USART2, buffer, length);
        select = USART_Read(USART2);
        
        // Checking if user would like to set a custom lower bound
        // and making sure the entry is valid
        if(select == 'y' || select == 'Y'){
            do {
                length = sprintf((char*)buffer, "Enter a valid integer within the bounds of 50 and 9950us: ");
                USART_Write(USART2, buffer, length);
                USART_Read_String(buffer);
                lower_bound = atoi((char *)buffer);
            }while(lower_bound < 50 || lower_bound > 9950);
        }

        
        // Initalize buckets
        for(b_index = 0; b_index < 101; b_index++){
            bucket[b_index] = 0;
        }

        upper_bound = 100 + lower_bound;
        length = sprintf((char*)buffer, "Lowerbound : %u\r\nUpperbound : %u\r\n", lower_bound, upper_bound);
        USART_Write(USART2,buffer,length);
        index = 0;

        // Entering Wave Read Loop
        do{
            if(TIM1_Has_Count() != 0){
                b_value = TIM1_Get_Time();
                delta = b_value - old_value;
                old_value = b_value;
                index++;
                if( lower_bound <= delta && delta <= upper_bound){
                    bucket[delta - lower_bound]++;
                }
            }
        }while(index < 1001);

        // Print Bucket Results
        length = sprintf((char *)buffer,"Final bucket count\r\n****************************\r\n");
        USART_Write(USART2, buffer, length);
        for(b_index = 0; b_index < 101; b_index++){
            if (bucket[b_index] != 0){
                length = sprintf((char *)buffer,"%dus count: %d\r\n", (lower_bound + b_index),bucket[b_index]);
                USART_Write(USART2, buffer, length);
            }
        }
    }
}

