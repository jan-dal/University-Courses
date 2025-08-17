#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

#define LED_STRIP PORTD

uint8_t convert(uint8_t n);
uint8_t debounce(uint8_t button);
uint8_t debounce_release();

int main(){
	
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
	DDRD |= 0xff;
	DDRC &= 0x00;
	PORTC |= (_BV(PC3) | _BV(PC4) | _BV(PC5));
	LED_STRIP = 0x00;

	uint8_t number = 0;
	uint8_t pressed = 0;
	
	while(1){
		
		if(!pressed){
			if(debounce(PC4)){
				number = 0;
			} else if(debounce(PC3)){
				number++;
			} else if(debounce(PC5)){
				number--;
			} else {
				continue;
			}
			pressed = 1;
			LED_STRIP = convert(number);

		} else {
			if(debounce_release()){
				pressed = 0;
			}
		}
	}
}

uint8_t convert(uint8_t n){
	return n^(n >> 1);
}

uint8_t debounce(uint8_t button){
    if(bit_is_clear(PINC, button)){
        _delay_ms(10);
        if(bit_is_clear(PINC, button)){
            return 1;
        }
    }
    return 0;
}
uint8_t debounce_release(){
    if(bit_is_set(PINC, PC3) && bit_is_set(PINC, PC4) && bit_is_set(PINC, PC5)){
        _delay_ms(10);
        if(bit_is_set(PINC, PC3) && bit_is_set(PINC, PC4) && bit_is_set(PINC, PC5)){
            return 1;
        }
    }
    return 0;
}