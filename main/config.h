



#ifndef _CONFIG_H_
#define _CONFIG_H_


#define 	DEBOUNCE_DELAY 	1500
#define 	OPR_SECTION    	4 /* 
					Entire Operation Section <Including time[1],
					temperature[2], humidity[3] and date[4]>.
					*/ 
#define 	SETUP_SECTION	5 /* 
					Entire Setup Section <Including hour[1],
					minute[2], day[3], month[4] and year[5]>.
					*/ 
#define 	OPR_TIME  		(8) /*
			The entire time (in s-seconds) is going to last
			for OPR_TIME
			*/ 
#define		F_OVERFLOW		((F_CPU) / (1024UL * 124) - 1) /*
			The number of times/frequency of the TIMER0 overflow
			in a single second
			*/
#define 	HOUR_OVF		(13)

#define 	MINUTE_OVF		(60)

#define  	MONTH_OVF 		(13)

#define  	YEAR_OVF_VAL	(2041)

#define 	YEAR_OVF_TRUE	(2010)

#define 	LONG_CLICK		(125) /*
			~0.8 millisec interval
			*/
#define 	SHORT_CLICK		(25)

#define  	BUZZER_OVF   	(F_OVERFLOW / 5)


static uint8_t DAY_OVF_ARRAY[] = {1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1};


static inline uint8_t DAY_OVF(uint8_t mnth, uint16_t yr){

	if((mnth != 0) && (DAY_OVF_ARRAY[--mnth])){
		if(mnth == 1){

			if((!(yr % 4) && (yr % 100)) || (!(yr % 400))){

				return (uint8_t)(30);
			}

			return (uint8_t)(29);
		}

		return (uint8_t)(32);
	}

	return (uint8_t)(31); 
}


#endif /* _CONFIG_H_ */