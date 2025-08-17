#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#define LED_PORT PORTD
#define LED_DDR DDRD
#define CYCLE_TIME 10000

void delay(uint16_t time);
// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc
  DIDR0 = _BV(ADC0D);
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

int main(){

  adc_init();
	
  LED_DDR |= _BV(PD4);

  uint16_t on = CYCLE_TIME;
  uint16_t off = 0;
  uint16_t values[33] = {0, 36, 76, 121, 171, 227, 289, 359, 436, 523, 619, 727, 847, 981, 1130, 1296, 1482, 1690, 1921, 2179, 2466, 2787, 3145, 3545, 3990, 4487, 5041, 5659, 6349, 7118, 7976, 8933, 10000};
  
	while(1){
		
		ADCSRA |= _BV(ADSC);
		loop_until_bit_is_clear(ADCSRA, ADSC);

		uint16_t v = ADC;

    on = values[v >> 5];
    off = CYCLE_TIME - on;

    LED_PORT |= _BV(PD4);

    delay(on);

    LED_PORT &= ~_BV(PD4);

    delay(off);
	
	}
}

// void delay(uint16_t c){

//   while(c-- != 0){
//     _delay_us(1);
//   }

// }

void delay(uint16_t time){

  for(uint8_t i=0; (i < 16) && (time > 0); i++){

    if((time & 0x1) == 1){
      switch(i){
        case 0:
          _delay_us(_BV(0));
          break;
        case 1:
          _delay_us(_BV(1));
          break;
        case 2:
          _delay_us(_BV(2));
          break;
        case 3:
          _delay_us(_BV(3));
          break;
        case 4:
          _delay_us(_BV(4));
          break;
        case 5:
          _delay_us(_BV(5));
          break;
        case 6:
          _delay_us(_BV(6));
          break;
        case 7:
          _delay_us(_BV(7));
          break;
        case 8:
          _delay_us(_BV(8));
          break;
        case 9:
          _delay_us(_BV(9));
          break;
        case 10:
          _delay_us(_BV(10));
          break;
        case 11:
          _delay_us(_BV(11));
          break;
        case 12:
          _delay_us(_BV(12));
          break;
        case 13:
          _delay_us(_BV(13));
          break;
        case 14:
          _delay_us(_BV(14));
          break;
        case 15:
          _delay_us(_BV(15));
          break;
      }
    }
    time >>= 1;
  }
}
