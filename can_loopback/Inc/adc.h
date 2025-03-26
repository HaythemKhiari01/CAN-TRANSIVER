/*
 * adc.h
 *
 *  Created on: Jan 1, 2025
 *      Author: hayth
 */
#include <stdint.h>
#ifndef ADC_H_
#define ADC_H_

void pa1_adc_init(void);
void start_converstion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
