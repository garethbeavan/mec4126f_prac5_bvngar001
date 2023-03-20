#define STM32F051
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"
#include <stdio.h>
#include <stdlib.h>

void main(void);
void display_on_LCD(uint8_t num);
void init_LEDs(void);
void display_on_LEDs(uint8_t num);
void init_switches(void);
void update_display(void);

#define SW1_pressed !(GPIOA -> IDR & 0b10)
#define SW2_pressed !(GPIOA -> IDR & 0b100)

uint8_t count = 0;

void main(void) {


	init_LCD();
	init_LEDs();
	init_switches();

	update_display();

	while (1)
	{
		if(SW1_pressed) {
			if(count == 255)count=0;
			else count++;

			update_display();
			delay(100000);
			while(SW1_pressed);
		}


		else if(SW2_pressed) {
			if(count == 0)count=255;
			else count--;

			update_display();
			delay(100000);
			while(SW2_pressed);
		}
	}
}

void display_on_LCD(uint8_t num) {
    lcd_command(CLEAR);
	char str[4]; 					// Assuming 8-bit integer can have a maximum of 3 digits
    sprintf(str, "%d", num); 		// Convert the integer to string
    lcd_putstring(str);			// Displays string to LCD
}


void init_LEDs(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Enable GPIOB clock
    GPIOB->MODER |= GPIO_MODER_MODER0_0 |
    				GPIO_MODER_MODER1_0 |
					GPIO_MODER_MODER2_0 |
					GPIO_MODER_MODER3_0 |
					GPIO_MODER_MODER4_0 |
					GPIO_MODER_MODER5_0 |
					GPIO_MODER_MODER6_0 |
					GPIO_MODER_MODER7_0;
}


void display_on_LEDs(uint8_t num) {
    GPIOB->ODR = num; // Set the output data register of GPIOB to the binary representation of num
}

void init_switches(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable GPIOA clock
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1); // Set SW1 and SW2 as inputs
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0;
}

void update_display(void) {
	display_on_LCD(count);
	display_on_LEDs(count);
}
