#include "stm32f4xx.h"
#include "can_driver.h"
//#include "uart.h"
#include "bsp.h"
#include "fpu.h"
#include "adc.h"
#include "timebase.h"
#include <stdio.h>

#define GPIODEN (1U<<3)
#define PIN14 (1U<<14)
#define LED_PIN PIN14

uint32_t temp_value;

uint8_t rx_data[2];


uint32_t tx_mailbox[3];

can_rx_header_typedef rx_header;
can_tx_header_typedef tx_header;

uint8_t count = 0;

void CAN1_RX0_IRQHandler(void)
{
	if((CAN1->RF0R & CAN_RF0R_FMP0) != 0U)
	{
		can_get_rx_message(CAN_RX_FIFO0, &rx_header, rx_data);
		count++;
	}
}

int main (void) {
	fpu_enable();
	timebase_init();
	//debug_uart_init();

	pa1_adc_init();
	start_converstion();
	//SystemClock_Config();
	can_gpio_init();
	can_parms_init(CAN_MODE_NORMAL);
	can_filter_config(0x544);
	can_start();
	//printf("TX IS READY.. /n");
	while (1) {
		temp_value = adc_read();
		tx_header.dlc = 2 ;
		tx_header.ext_id = 0;
		tx_header.ide = CAN_ID_STD;
		tx_header.rtr =  0;
		tx_header.std_id =  0x244;
		tx_header.transmit_global_time = 0;
		uint8_t tx_msg_pack[2]={0};
		tx_msg_pack[0] = (temp_value >> 8) & 0xFF;
		tx_msg_pack[1] = temp_value & 0xFF;
        can_add_tx_message(&tx_header,tx_msg_pack,tx_mailbox);

        delay(1);

	}
}
