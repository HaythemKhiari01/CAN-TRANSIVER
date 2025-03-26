
#ifndef BSP_H_
#define BSP_H_

#include <stdint.h>
#include <stdbool.h>
#define PIN12 (1U<<12)
#define LED_PIN12 PIN12

#define GPIOAEN (1U<<0)
#define PIN0 (1U<<0)
#define BTN_PIN PIN0

void led_init(void);
void led_on(void);
void led_off(void);
void button_init(void);
bool get_btn_state(void);

#endif /* BSP_H_ */
