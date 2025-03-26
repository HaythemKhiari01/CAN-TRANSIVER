#include "stm32f4xx.h"
#include "can_driver.h"

uint8_t rxData[10];
can_rx_header_typedef rx_header;
uint8_t count=0;

void CAN1_RX0_IRQHandler(void) {
	if ((CAN1->RF0R & CAN_RF0R_FMP0)!=0U){
		can_get_rx_message(CAN_RX_FIFO0,rx_header,rxData);
count ++;
	}
}

int main (void) {
	while (1) {

	}
}
