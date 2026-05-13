/*
 * Original Author: Dauda Muazu Sulaiman
 */

// ---------------
//   Pin Defines
// ---------------

/*
	LED RELATED PIN_DEFS...
*/
#define		LED_LIVE_PORT		PORTD
#define 	LED_GROUND_PORT 	PORTB
#define 	LED_LIVE_DDR 		DDRD
#define 	LED_GROUND_DDR 		DDRB
/*	GPx: Ground Pin Bit		*/
#define 	LED_GP1				PB0
#define 	LED_GP2				PB1
#define     LED_GP3 			PB2
#define  	LED_GP4 			PB3


/*
	DHT_11 RELATED PIN_DEFS

		The numbers were chosen based on what was described
		as PORT and BIT selection in the 'DHT_11.h' header
		file.
*/
#define 	DHT_PORT 			_PORT_B /* For PORTB selection */
#define 	DHT_PIN 			P4 		/* For PB4 selection */


/*
	POT RELATED PIN_DEFS
*/
#define 	POT_PORT		PORTC
#define 	POT_PIN 		PINC
#define 	POT_DDR  		DDRC
#define 	POT 			PC0


/*
	BUZZER RELATED PIN_DEFS
*/
#define 	BUZZER_PORT 	PORTB
#define 	BUZZER_PIN 		PINB
#define 	BUZZER_DDR 		DDRB
#define 	BUZZER 			PB5


/*
	PUSH BUTTON RELATED PIN_DEFS

	PB: Push Button
*/
#define 	PB_PORT			PORTC
#define 	PB_PIN 			PINC
#define 	PB_DDR 			DDRC
#define 	PUSH_BUTTON 	PC1


/*
	LED_ARRAY RELATED PIN_DEFS

	LAx: Led Array Bit
*/
#define 	LA_PORT 		PORTC
#define 	LED_DDR  		DDRC
#define 	LA_PIN 			PINC
#define 	LA1 			PC2
#define 	LA2				PC3
#define 	LA3 			PC4
#define 	LA4 			PC5