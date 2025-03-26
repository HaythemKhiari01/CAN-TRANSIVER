#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_
#include "stm32f4xx.h"
#include <stdint.h>

#define CAN_ID_STD (0x00)
#define CAN_ID_EXT (0x01)

#define CAN_RX_FIFO0	0x00
#define CAN_RX_FIFO1	0x01

typedef struct {
	uint32_t std_id;
	uint32_t ext_id;
	uint32_t ide;
	uint32_t rtr;
	uint32_t dlc;
	uint8_t transmit_global_time;

	}can_tx_header_typedef;

typedef struct {
		uint32_t std_id;
		uint32_t ext_id;
		uint32_t ide;
		uint32_t rtr;
		uint32_t dlc;
		uint32_t timestamp;
		uint32_t filter_match_index;

		}can_rx_header_typedef;
void can_gpio_init(void);
void can_parms_init(uint8_t mode);
void can_start(void) ;
uint8_t can_add_tx_message(can_tx_header_typedef *pHeader, uint8_t aData[], uint32_t *pTxMailbox);
uint8_t can_get_rx_message(uint32_t RxFifo, can_rx_header_typedef *pHeader, uint8_t aData[]);
void can_filter_config(uint16_t std_id);


#endif /* CAN_DRIVER_H_ */
