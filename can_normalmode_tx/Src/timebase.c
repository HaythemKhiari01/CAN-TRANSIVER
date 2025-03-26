#include "timebase.h"
#include "stm32f4xx.h"

#define CTRL_ENABLE		(1U<<0)
#define CTRL_TICKINT	(1U<<1)
#define CTRL_CLCKSRC	(1U<<2)
#define CTRL_COUNTFLAG	(1U<<16)

#define ONE_SEC_LOAD	 16000000

#define MAX_DELAY		 0xFFFFFFFF

#define GPIOHEN (1U<<7)

#define TICK_FREQ		 1

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;


/*Delay in seconds*/
void delay(uint32_t delay)
{
	uint32_t tickstart =  get_tick();
	uint32_t wait =  delay;

	if(wait < MAX_DELAY)
	{
		wait += (uint32_t)TICK_FREQ;
	}

    while((get_tick() - tickstart) < wait){}

}

uint32_t get_tick(void)
{
	__disable_irq();
	g_curr_tick_p = g_curr_tick;
	__enable_irq();

	return g_curr_tick_p;

}
static void tick_increment(void)
{
	g_curr_tick += TICK_FREQ;
}
void timebase_init(void)
{

	/*Disable global interrupts*/
	__disable_irq();

    /*Load the timer with number of clock cycles per second*/
	SysTick->LOAD =  ONE_SEC_LOAD - 1;

    /*Clear systick current value register*/
	SysTick->VAL = 0;

    /*Select internal clock source*/
	SysTick->CTRL = CTRL_CLCKSRC;

    /*Enable interrupt*/
	SysTick->CTRL |= CTRL_TICKINT;

	/*Enable systick*/
	SysTick->CTRL |=CTRL_ENABLE;

	/*Enable global interrupts*/
	__enable_irq();
}

void SysTick_Handler(void)
{
	tick_increment();
}



/*void SystemClock_Config(void) {
	// Enable clock acess to GPIOH
		RCC->AHB1ENR |= GPIOHEN;
		//Set PH0 and PH1 to alternate function mode
		GPIOH->MODER &=~(1U<<0);
		GPIOH->MODER |=(1U<<1);
		GPIOH->MODER &=~(1U<<2);
		GPIOH->MODER |=(1U<<3);
	// 1 Activer HSE
	RCC->CR |= RCC_CR_HSEON ;
	// 2 Wait until HSE is Ready
	while (!(RCC->CR & RCC_CR_HSERDY)) {}
	// 3 Configuration du PLL Le registre RCC_PLLCFGR sert à configurer la PLL (Phase-Locked Loop) du STM32, qui permet d'augmenter ou d'ajuster la fréquence d'horloge du microcontrôleur.
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
	RCC->PLLCFGR |= (8<<0);// (HSE=8)/8 = 1MHZ
	RCC->PLLCFGR |= (336<<6); // 1 MHZ * 336 = 336MHZ
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;
	RCC->PLLCFGR |= (0<<16); // 336/2 = 168 MHZ
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	RCC->PLLCFGR |= (1<<22); // PLL source = HSE
	// 4 Activer PLL
	RCC->CR |= RCC_CR_PLLON ;
	while (!(RCC->CR & RCC_CR_PLLRDY)) {}
	// 5 Configurer AHB et APB
	RCC->CFGR |= (0<<4);// AHB1= 168/1 = 168MHZ
	RCC->CFGR |= (5<<10);// APB1 = 168/4= 42 MHZ
    // 6 Sélectionner le PLL comme source SYSCLK
	RCC->CFGR |= (2<<0);

	FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN ;
	FLASH->ACR &=~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS ;
	// 7 Verifier si PLL non séléctionné
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

}
*/
