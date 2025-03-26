#include <stm32f4xx.h>
#include "bsp.h"

#define GPIODEN (1U<<3)
#define PIN14 (1U<<14)
#define LED_PIN PIN14


#define GPIOAEN (1U<<0)
#define PIN0 (1U<<0)
#define BTN_PIN PIN0

void led_init(void) {
	RCC->AHB1ENR |=GPIODEN;
	GPIOD->MODER |= (1U<<28);
	GPIOD->MODER &=~(1U<<29);
}

void led_on(void) {
	GPIOD->ODR |= LED_PIN;
}

void led_off(void) {
	GPIOD->ODR &=~ LED_PIN;
}

void button_init(void)
{

	RCC->AHB1ENR |=GPIOAEN;

	GPIOA->MODER &=~ (1U<<00);
	GPIOA->MODER &=~(1U<<01);

}
bool get_btn_state(void)
{

	if(GPIOA->IDR & BTN_PIN)
	{
		return false;
	}
	else
	{
		return true;
	}

}

