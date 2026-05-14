/*
 * MAIN.C - AVR Digital Watch Firmware Source
 * * Copyright (c) 2026 Dauda Muazu Sulaiman
 * * Copyright (c) 2026 KibrisOrder                <https://ss.kibrisorder.com>
 * * Author: Dauda Muazu Sulaiman
 * * Organization: KibrisOrder
 * * All rights reserved.
 * * This software is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */



#include<avr/io.h>
#include<avr/interrupt.h>
#include<avr/power.h>
#include<util/atomic.h>

#include "4D_7S.h"
#include "DHT_11.h"
#include "reg_defs_t.h"
#include "pinDefines.h"
#include "config.h"



volatile uint8_t setup_flag = 0;
volatile uint8_t setup_click = 0;
volatile uint8_t setup_update = 0;
volatile uint8_t setup_increase = 0;

volatile uint8_t pm_indicator = 0;
volatile static uint8_t hour = 10;
volatile static uint8_t minute = 42; 
volatile static uint8_t seconds = 0;
volatile static uint8_t day = 11;
volatile static uint8_t month = 5;
volatile static uint16_t year = 2026;
volatile static uint8_t day_ovf = 0;

volatile uint8_t button_state = 0;
volatile uint8_t button_click = 0;

volatile uint16_t isr_flag = 0;
volatile uint8_t buzzer_flag = 0;
volatile uint8_t buzzer_counter = 0;
volatile uint8_t endRun_flag = 0;

volatile uint8_t power_on = 0;
volatile uint8_t ovf_counter = 0;

static uint8_t temp_ = 0;
static uint8_t humd_ = 0;
volatile uint8_t update_tnh = 0;

volatile uint16_t display_number = 0;
volatile uint8_t display_on = 0;


static uint8_t _4D_7S_Ground[] = {LED_GP1, LED_GP2, LED_GP3, LED_GP4};


static inline void initTimer0(void);
static inline void init4D_7S(void);
static inline void endRun(void);

static void setupWatch(void);
static inline void handleSetup(void);


/* ISR (On PORTB) From the DHT_11.h setup */
ISR(PCINT0_vect){
 
	DHT_HandleSignal();
}


/* ISR for updating the Time and date */
ISR(_TIMER0_COMPA_){

	if((++ovf_counter) >= F_OVERFLOW) {

	    ovf_counter = 0;

		seconds = (seconds + 1) % 60;

		if(++power_on >= OPR_TIME){
			power_on = 0;

			if(endRun_flag == 1){
				endRun_flag = 0;
			}
		}

		if(!(seconds)){
			minute = (minute + 1) % MINUTE_OVF;

			if(!(minute)){

				if((++hour) == HOUR_OVF){
					hour = 1;
					pm_indicator ^= 1;
				}

				update_tnh = 1;
			}
		}
	}

	if((buzzer_flag) && ((++buzzer_counter) >= BUZZER_OVF)){

		BUZZER_PORT &= ~(1 << BUZZER);
		buzzer_flag = 0;
		buzzer_counter = 0;
	}

	if(bit_is_clear(PB_PIN, PUSH_BUTTON)){

		isr_flag++;
	}

	if(isr_flag){

		if(bit_is_set(PB_PIN, PUSH_BUTTON)){

			uint16_t check_isr = 0;

			ATOMIC_BLOCK(ATOMIC_FORCEON){
				check_isr = isr_flag;
			}
			 
			if((!setup_flag) && ((check_isr < (LONG_CLICK)) && (check_isr > SHORT_CLICK))){
				
					button_state = 1;
					setup_update = 0;
					setup_flag = 0;
					setup_click = 0;
				
			}else if(display_on && (check_isr > SHORT_CLICK)){

				if((check_isr < (LONG_CLICK))){
					setup_increase = 1;
				}

				setup_update = 1;
				setup_flag = 1;
				button_state = 0;
				button_click = 0;
			}

			isr_flag = 0;
		}
	} 
}



int main(void){

	clock_prescale_set(clock_div_1);

	initTimer0();

	/* 4D_7S Display Setup */
	initLED_DISPLAY(&LED_LIVE_PORT, &LED_GROUND_PORT, 2); /* Decimal Point on 2nd Number */
	setPin(_4D_7S_Ground); 
 
	/* DHT_11 Setup */
	DHT_Init(DHT_PORT, DHT_PIN);

	temp_ = DHT_Get_Temp();
	humd_ = DHT_Get_Humidity();

	PB_DDR &= ~(1 << PUSH_BUTTON);
	PB_PORT |= (1 << PUSH_BUTTON);

	BUZZER_DDR |= (1 << BUZZER);

	day_ovf = DAY_OVF(month, year);

	while(1){

		if(button_state){

			if(!display_on){
				init4D_7S();
				BUZZER_PORT |= (1 << BUZZER);
			}
			
			power_on = 1;

			switch(button_click){
				case 0:{
					display_number = (hour*100) + (minute);
					DISPLAY(display_number, pm_indicator);
					break;
				}
				case 1:{
					DISPLAY_wChar(0x7840, temp_);
					break;
				}
				case 2:{
					DISPLAY_wChar(0x7440, humd_);
					break;
				}
				case 3:{
					display_number = (day*100) + (month);
					DISPLAY_nDP(display_number);
					break;
				}
			}

			if((++button_click) >= (OPR_SECTION)){
				button_click = 0;
			}

			button_state = 0;
		}

		if(setup_flag){

			setupWatch();
		}

		if(update_tnh){

			temp_ = DHT_Get_Temp();
			humd_ = DHT_Get_Humidity();

			update_tnh = 0;
		}

		if(!power_on && !endRun_flag){
			DISPLAY_reset();
			endRun();
		}

	}

	return 0;
}



static inline void initTimer0(void){

	_OCR0A_ = (uint8_t)((F_CPU / (1024UL * 125UL)) - 1);

	_TCR0A_ |= (1 << _WGM01_);
	_TCR0B_ |= (1 << _CS02_) | (1 << _CS00_);

	_TIMSK0_ |= (1 << _OCIE0A_);

	sei();
}


static inline void init4D_7S(void){

	LED_LIVE_DDR |= (0xff);
	LED_GROUND_DDR |= (0xf);

	endRun_flag = 1;
	buzzer_counter = 0;
	buzzer_flag = 1;
	display_on = 1;
}


static inline void endRun(void){

	endRun_flag = 2;

	buzzer_counter = 0;
	buzzer_flag = 1;
	BUZZER_PORT |= (1 << BUZZER);

	LED_LIVE_DDR &= ~(0xff);
	LED_GROUND_DDR &= ~(0xf);

	setup_flag = 0;
	setup_update = 0;
	setup_click = 0;

	button_click = 0;
	button_state = 0;

	display_on = 0;
	power_on = 0;
	isr_flag = 0;
}


static inline void handleSetup(void){

	switch(--setup_click){
		case 0:{
			if((++hour) >= (HOUR_OVF)){
				hour = 1;
				pm_indicator ^= 1;
			}
			break;
		}
		case 1:{
			if((++minute) >= (MINUTE_OVF)){
				minute = 0;
			}
			break;
		}
		case 2:{
			if((++day) >= (day_ovf)){
				day = 1;
			}
			break;
		}
		case 3:{
			if((++month) >= (MONTH_OVF)){
				month = 1;
			}
			day_ovf = DAY_OVF(month, year);
			break;
		}
		case 4:{
			if((++year) >= (YEAR_OVF_VAL)){
				year = (YEAR_OVF_TRUE);
			}
			break;
		}
	}
}


static void setupWatch(void){

	if(setup_update){

		if(setup_increase){
			if(setup_click){
				handleSetup();
			}
		}

		power_on = 1;

		switch(setup_click){
			case 0:{
				display_number = (hour*100) + (minute);
				DISPLAY_flick(display_number, 12, 0, pm_indicator);
				break;
			}
			case 1:{
				display_number = (hour*100) + (minute);
				DISPLAY_flick(display_number, 34, 0, pm_indicator);
				break;
			}
			case 2:{
				display_number = (day*100) + (month);
				DISPLAY_flick(display_number, 12, 1, 0);
				break;
			}
			case 3:{
				display_number = (day*100) + (month);
				DISPLAY_flick(display_number, 34, 1, 0);
				break;
			}
			case 4:{
				ATOMIC_BLOCK(ATOMIC_FORCEON){
					display_number = (year);
				}
				DISPLAY_flick(year, 1234, 1, 0);
				break;
			}
		}

		if((++setup_click) > (SETUP_SECTION)){
			setup_click = 0;
			setup_flag = 0;
			button_state = 1;
		}

		setup_increase = 0;
		setup_update = 0;
	}

}