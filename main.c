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
void initInterrupts(void);

#define SW1_pressed !(GPIOA -> IDR & 0b10)
#define SW2_pressed !(GPIOA -> IDR & 0b100)

uint8_t count = 0;
uint8_t SW3_count = 0;

void main(void) {

	init_LCD();
	init_LEDs();
	init_switches();
	initInterrupts();

	update_display();

	while (1)
	{
		if ((SW3_count%2)==0)
		{
			if(SW1_pressed)
			{
				if(count == 255)count=0;
				else count++;

				update_display();
				delay(50000);
				while(SW1_pressed);
			}


			else if(SW2_pressed)
			{
				if(count == 0)count=255;
				else count--;

				update_display();
				delay(50000);
				while(SW2_pressed);
			}
		}

		else
		{
			lcd_command(CLEAR);
			display_on_LEDs(0);
			delay(5000);
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
    GPIOA->MODER &= ~(GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3); // Set SW1 and SW2 as inputs
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0;
}

void update_display(void) {
	display_on_LCD(count);
	display_on_LEDs(count);
}


void initInterrupts(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
	EXTI->IMR |= EXTI_IMR_MR3;
	EXTI->FTSR |= EXTI_FTSR_TR3;

	NVIC_EnableIRQ(EXTI2_3_IRQn);
}

void EXTI2_3_IRQHandler(void)
{
	SW3_count +=1;
	EXTI -> PR |= EXTI_PR_PR3;
}
