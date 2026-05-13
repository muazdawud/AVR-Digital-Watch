/*
 * 4D_7S Library - High-performance 4-Digit 7-Segment Multiplexing
 * * Copyright (c) 2026 Dauda Muazu Sulaiman
 * Copyright (c) 2026 KibrisOrder                <https://ss.kibrisorder.com>
 * * Author: Dauda Muazu Sulaiman
 * Organization: KibrisOrder
 * All rights reserved.
 * * This software is licensed under the MIT License.
 * * See the LICENSE file in the project root for full license information.
 * 
 * See the README file in the project root to understand how to use the software.
 */


#ifndef _4D_7S_H
#define _4D_7S_H

/*
	#define ENABLE_DECIMAL_POINT_ALL
	#define ENABLE_DECIMAL_POINT_1
	#define ENABLE_DECIMAL_POINT_2
	#define ENABLE_DECIMAL_POINT_3
	#define ENABLE_DECIMAL_POINT_4
	#define DISABLE_DECIMAL_POINT
*/

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint8_t *LED_DISPLAY_PORT;
extern volatile uint8_t *LED_DISPLAY_GROUND;

void setPin(const uint8_t ground[]);

void initLED_DISPLAY(volatile uint8_t *PORT_1, volatile uint8_t *PORT_2, uint8_t dp_mask);

void DISPLAY(uint16_t num, uint8_t pm_check);

/*
	Go to the source implementation for some explanations
	*/
void DISPLAY_wChar(uint16_t character, uint8_t number);

void DISPLAY_nDP(uint16_t num);

void DISPLAY_flick(uint16_t number, uint16_t flick_number, uint8_t disable_dp, uint8_t pm_check);

void DISPLAY_reset();

#ifdef __cplusplus
}
#endif

#endif