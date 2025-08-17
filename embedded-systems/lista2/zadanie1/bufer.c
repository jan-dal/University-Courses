#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED PB5
#define BUTTON PC4

int8_t bufor[1000];
int8_t debounce();

int main() {

    LED_DDR |= _BV(LED);
    PORTC |= _BV(BUTTON);

    uint16_t index = 0;

    while(1){
        
        for(int8_t i=0; i < 1000; i++){
            if(bufor[index]){
                LED_PORT |= _BV(LED);
                bufor[index] = 0;
            } else {
                LED_PORT &= ~_BV(LED);
            }

            if(debounce()){
                bufor[index] = 1;
            } else {
		_delay_ms(1);	    
	    }

            index++;
            if(index >= 1000){
                index = 0;
            }
        }
    }
}

int8_t debounce(){
    if(bit_is_clear(PINC, BUTTON)){
        _delay_ms(1);
        if(bit_is_clear(PINC, BUTTON)){
            return 1;
        }
    }
    return 0;
}
