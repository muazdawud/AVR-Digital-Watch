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


#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#include "4D_7S.h"
#include "reg_defs_t.h"

#define GROUND_SIZE 4

volatile uint8_t *LED_DISPLAY_PORT;
volatile uint8_t *LED_DISPLAY_GROUND;

volatile static uint8_t groundCount = 0;

volatile static uint8_t pattern = 0;

static uint8_t numberArray[GROUND_SIZE];

static uint8_t GROUND_ARRAY[GROUND_SIZE];

static uint8_t DPF[GROUND_SIZE] = {0, 0, 0, 0};

static uint8_t temp_DPF[GROUND_SIZE] = {0, 0, 0, 0};

static volatile uint16_t second_timing = 0;

static volatile uint8_t dp_check = 0;

static uint16_t compare_val = 0;

static uint16_t mid_cycle_compare = 0;

static uint8_t Flick_Array[GROUND_SIZE] = {0, 0, 0, 0};

volatile static uint8_t Flick_Flag = 0;

volatile static uint8_t dp_disable_flag = 0;


// THIS IS THE ARRAY WITHOUT DECIMAL POINTS
static const uint8_t NUMBER_BYTES[10] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7D, // 6
	0x07, // 7
	0x7F, // 8
	0x6F  // 9
};


ISR(_TIMER2_COMPA_) {

    if((++second_timing) >= mid_cycle_compare){
    	second_timing = 0;
    	dp_check ^= (1);
    	Flick_Flag ^= (1);
    }

	for (uint8_t i = 0; i < 4; i++) {
        *LED_DISPLAY_GROUND |= (1 << GROUND_ARRAY[i]);
    }

    pattern = numberArray[groundCount];

    if(DPF[groundCount] && (dp_check)){

    	pattern |= 0x80;
    }

    if(Flick_Array[groundCount] && !Flick_Flag){

    	*LED_DISPLAY_PORT = 0;
    }else{

    	*LED_DISPLAY_PORT = pattern;
    }

	*LED_DISPLAY_GROUND &= ~(1 << GROUND_ARRAY[groundCount]);

	if(++groundCount > 3){
		groundCount = 0;
	}
}


static inline void initTIMER_2(void) {
    	/* 
			Changed the value that multiplies by F_CPU to 0.002
			To decrease ISR call latency and boost free clock cycles
			*/
        _OCR2A_ = (uint8_t)((0.002 * F_CPU) / 1024 - 1);

		_TCR2A_ |= (1 << _WGM21_);

		compare_val = ((F_CPU / 1024UL) / (_OCR2A_ + 1));
		mid_cycle_compare = (compare_val / 2);

		_TIMSK2_ |= (1 << _OCIE2A_);
}


static void extractNumber(uint16_t number) {

	cli();

	dp_check = 1;

    for(uint8_t i = 3; i < 255; i--){
    	uint8_t digit = number % 10;

    	numberArray[i] = NUMBER_BYTES[digit];

    	number /= 10;
    }

    sei();
}


void setPin(const uint8_t ground[]){

	for(uint8_t i = 0; i < GROUND_SIZE; i++){

		GROUND_ARRAY[i] = ground[i];
	}
}


void initLED_DISPLAY(volatile uint8_t *PORT_1, volatile uint8_t *PORT_2, uint8_t dp_mask){

	LED_DISPLAY_PORT = PORT_1;
	LED_DISPLAY_GROUND = PORT_2;

	//WITH THIS, A USER CAN PASS '0' IF HE DOESN'T WANT DPs

	if((--dp_mask) < (GROUND_SIZE)){
		DPF[dp_mask] = 1;
	}

	if((dp_mask) == 255){
		for(uint8_t i = 0; i < GROUND_SIZE; i++){
			DPF[i] = 0;
		}
	}

	initTIMER_2();
}
 

void DISPLAY(uint16_t num){

	DISPLAY_reset();

	extractNumber(num);

	_TCR2B_ |= (1 << _CS22_) | (1 << _CS21_) | (1 << _CS20_);
	TCNT2 = 0x1E;
}


/* 
	Should be updated to display characters at given/custom position with 
	This isn't and would not be made to be generic, it is fully custom and
	made for the need of showing characters

	==========||> [-CUSTOM FUNCTION BUILD-] <||==========
*/

static void disable_decimal(){

	for(uint8_t i = 0; i < GROUND_SIZE; i++){
		temp_DPF[i] = DPF[i];
		DPF[i] = 0;
	}

	dp_disable_flag = 1;
}

void DISPLAY_wChar(uint16_t character, uint8_t number){

	DISPLAY_reset();
    
    extractNumber(number);

    numberArray[0] = (character >> 8);
    numberArray[1] = (character);

    disable_decimal();

	_TCR2B_ |= (1 << _CS22_) | (1 << _CS21_) | (1 << _CS20_);
	TCNT2 = 0x1E;
}

void DISPLAY_nDP(uint16_t num){

	DISPLAY_reset();

	extractNumber(num);

	disable_decimal();

	_TCR2B_ |= (1 << _CS22_) | (1 << _CS21_) | (1 << _CS20_);
	TCNT2 = 0x1E;
}

void DISPLAY_flick(uint16_t number, uint16_t flick_number, uint8_t disable_dp){
	
	DISPLAY_reset();

	extractNumber(number);

	if(disable_dp){
		disable_decimal();
	}

	while(flick_number){

		uint8_t digit = flick_number % 10;

		if((digit > 0) && (digit < 5)){
			Flick_Array[digit-1] = 1;
		}

		flick_number /= 10;
	}

    Flick_Flag = 1;

	_TCR2B_ |= (1 << _CS22_) | (1 << _CS21_) | (1 << _CS20_);
	TCNT2 = 0x1E;
}

void DISPLAY_reset(){

	cli();

	_TCR2B_ &= ~(1 << _CS22_) & ~(1 << _CS21_) & ~(1 << _CS20_);
	
	for(uint8_t i = 0; i < GROUND_SIZE; i++){

		Flick_Array[i] = 0;

		if(dp_disable_flag){
		
			DPF[i] = temp_DPF[i];
		}
	}

	groundCount = 0;
	second_timing = 0;
	pattern = 0;
	dp_check = 0;
	Flick_Flag = 0;
	dp_disable_flag = 0;

	sei();
}
